#ifndef ILISTABLE_H
#define ILISTABLE_H


class IListable {
public:
    virtual void activateListing()   = 0;
    virtual void deactivateListing() = 0;
    virtual bool isActive() const    = 0;
    virtual void showListing() const = 0;

    virtual ~IListable();
};

#endif
