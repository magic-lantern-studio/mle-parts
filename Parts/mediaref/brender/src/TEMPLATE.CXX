#include <mle/MleMediaRef.h>
#include <mle/refucvt.h>
#include TEMPLATE_CLASS_HEADER

MLE_MEDIAREF_SOURCE(TEMPLATE_CLASS,TEMPLATE_PARENT);

TEMPLATE_CLASS::TEMPLATE_CLASS()
{
	// Replace the old ref converter with one that understands URLs.
	delRefConverter();
	setRefConverter(new MleReferenceUrlConverter);
}
