#ifndef IPAYABLE_H
#define IPAYABLE_H

// ==================== ABSTRACT CLASS: IPayable ====================
// Enforces a common payment contract on any class that handles transactions.

class IPayable {
public:
    virtual void processPayment()  = 0;
    virtual void refundPayment()   = 0;
    virtual void generateInvoice() = 0;

    virtual ~IPayable();
};

#endif
