#include <iostream>
#include <iomanip>
using namespace std;

//Account base class
class Account {
protected:
    double balance;
    int numDeposits;
    int numWithdrawals;
    double annualInterestRate;
    double monthlyServiceCharges;

public:
    Account(double bal = 0.0, double rate = 0.0)
        : balance(bal),
          annualInterestRate(rate),
          numDeposits(0),
          numWithdrawals(0),
          monthlyServiceCharges(0.0)
    {}

    virtual void deposit(double amount) {
        balance += amount;
        numDeposits++;
    }

    virtual void withdraw(double amount) {
        balance -= amount;
        numWithdrawals++;
    }

    virtual void calcInt() {
        double monthlyRate = annualInterestRate / 12.0;
        double monthlyInterest = balance * monthlyRate;
        balance += monthlyInterest;
    }

    virtual void monthlyProc() {
        balance -= monthlyServiceCharges;
        calcInt();
        numWithdrawals = 0;
        numDeposits = 0;
        monthlyServiceCharges = 0;
    }

    double getBalance() const { return balance; }
    int getNumDeposits() const { return numDeposits; }
    int getNumWithdrawals() const { return numWithdrawals; }
    double getMonthlyServiceCharges() const { return monthlyServiceCharges; }

protected:
    void addServiceCharge(double amount) {
        monthlyServiceCharges += amount;
    }
};


//savings class
//true = active, false = inactive
class SavingsAccount : public Account {
private:
    bool status;

    void updateStatus() {
        status = (balance >= 25.0);
    }

public:
    SavingsAccount(double bal = 0.0, double rate = 0.0)
        : Account(bal, rate)
    {
        updateStatus();
    }

    virtual void withdraw(double amount) override {
        updateStatus();
        if (!status) {
            cout << "Savings account is inactive, unable to withdraw." << endl;
            return;
        }
        Account::withdraw(amount);
        updateStatus();
    }

    virtual void deposit(double amount) override {
        Account::deposit(amount);
        if (!status && balance >= 25.0) {
            status = true;
        }
    }

    virtual void monthlyProc() override {
        if (numWithdrawals > 4) {
            int excess = numWithdrawals - 4;
            monthlyServiceCharges += excess * 1.0;
        }
        Account::monthlyProc();
        updateStatus();
    }
};

//checking class
class CheckingAccount : public Account {
public:
    CheckingAccount(double bal = 0.0, double rate = 0.0)
        : Account(bal, rate) {}
		
	//$15 fee for overdraft
    virtual void withdraw(double amount) override {
        if (balance - amount < 0) {
            cout << "Insufficient funds. $15 Overdraft fee charged." << endl;
            monthlyServiceCharges += 15.0;
        } else {
            Account::withdraw(amount);
        }
    }
	
	//monthly fee - $5 + $0.10 per withdrawal
    virtual void monthlyProc() override {
        monthlyServiceCharges += 5.0;
        monthlyServiceCharges += 0.10 * numWithdrawals;
        Account::monthlyProc();
    }
};

//print monthly report
void printMonthlyReport(const string& name,
                        double startingBalance,
                        const Account& acct)
{
    cout << fixed << setprecision(2);
    cout << "\n----- " << name << " Monthly Report -----\n";
    cout << "Starting Balance:      $" << startingBalance << endl;
    cout << "Deposits:               " << acct.getNumDeposits() << endl;
    cout << "Withdrawals:            " << acct.getNumWithdrawals() << endl;
    cout << "Charges:               $" << acct.getMonthlyServiceCharges() << endl;
    cout << "Final Balance:         $" << acct.getBalance() << endl;
}

//main
int main() {
    double savBal, chkBal, annualRate;
    cout << "Enter savings account starting balance: ";
    cin >> savBal;
    cout << "Enter checking account starting balance: ";
    cin >> chkBal;
    cout << "Enter annual interest rate (ex. 0.04 -> 4%): ";
    cin >> annualRate;

    SavingsAccount savings(savBal, annualRate);
    CheckingAccount checking(chkBal, annualRate);

    double startSav = savBal;
    double startChk = chkBal;

    int n;
    double amt;

    // savings transaction
    cout << "\nEnter number of deposits for the savings account: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Deposit #" << (i + 1) <<" amount: ";
        cin >> amt;
        savings.deposit(amt);
    }

    cout << "Enter number of withdrawals from the savings account: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Withdrawal #" << (i + 1) <<" amount: ";
        cin >> amt;
        savings.withdraw(amt);
    }

    savings.monthlyProc();

    // checking transaction
    cout << "\nEnter number of deposits for the checking account: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Deposit #" << (i + 1) <<" amount: ";
        cin >> amt;
        checking.deposit(amt);
    }

    cout << "Enter number of withdrawals from the checking account: ";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cout << "Withdrawal #" << (i + 1) <<" amount: ";
        cin >> amt;
        checking.withdraw(amt);
    }

    checking.monthlyProc();

    //print reports
    printMonthlyReport("Savings Account", startSav, savings);
    printMonthlyReport("Checking Account", startChk, checking);

    return 0;
}
