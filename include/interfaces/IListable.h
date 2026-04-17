#ifndef ILISTABLE_H
#define ILISTABLE_H

// ==================== ABSTRACT CLASS: IListable ====================
// Any object that can be posted as a marketplace listing must implement this.
// Provides a uniform activate/deactivate interface.

class IListable {
public:
    virtual void activateListing()   = 0;
    virtual void deactivateListing() = 0;
    virtual bool isActive() const    = 0;
    virtual void showListing() const = 0;

    virtual ~IListable();
};

#endif
