#ifndef TEMPLATE_IFDEF
#define TEMPLATE_IFDEF

// Include Runtime Player header files.
#include <mle/MleMediaRef.h>

// Include parent class header file.
#include TEMPLATE_PARENT_HEADER

class TEMPLATE_CLASS : public TEMPLATE_PARENT
{
    MLE_MEDIAREF_HEADER(TEMPLATE_CLASS);
    
  // These are all we need from the base class.
  public:
    TEMPLATE_CLASS();
};
#endif /* TEMPLATE_IFDEF */
