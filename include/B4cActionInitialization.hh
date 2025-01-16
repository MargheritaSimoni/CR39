#ifndef B4cActionInitialization_h
#define B4cActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.
///

class B4cActionInitialization : public G4VUserActionInitialization
{
  public:
    B4cActionInitialization();
    virtual ~B4cActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif

    
