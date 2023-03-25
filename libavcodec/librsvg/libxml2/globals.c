/*
 * globals.c: definition and handling of the set of global variables
 *            of the library
 *
 * The bottom of this file is automatically generated by build_glob.py
 * based on the description file global.data
 *
 * See Copyright for the status of this software.
 *
 * Gary Pennington <Gary.Pennington@uk.sun.com>
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "xmlmemory.h"
#include "threads.h"

#include "private/error.h"
#include "private/globals.h"
#include "private/threads.h"
#include "private/tree.h"

/* #define DEBUG_GLOBALS */

/*
 * Helpful Macro
 */
#ifdef LIBXML_THREAD_ENABLED
#define IS_MAIN_THREAD (xmlIsMainThread())
#else
#define IS_MAIN_THREAD 1
#endif

/*
 * Mutex to protect "ForNewThreads" variables
 */
static xmlMutex xmlThrDefMutex;

/**
 * xmlInitGlobals:
 *
 * DEPRECATED: Alias for xmlInitParser.
 */
void xmlInitGlobals(void) {
    xmlInitParser();
}

/**
 * xmlInitGlobalsInternal:
 *
 * Additional initialisation for multi-threading
 */
void xmlInitGlobalsInternal(void) {
    xmlInitMutex(&xmlThrDefMutex);
}

/************************************************************************
 *									*
 *	All the user accessible global variables of the library		*
 *									*
 ************************************************************************/

/*
 * Memory allocation routines
 */
#undef	xmlFree
#undef	xmlMalloc
#undef	xmlMallocAtomic
#undef	xmlMemStrdup
#undef	xmlRealloc

#if defined(DEBUG_MEMORY_LOCATION) || defined(DEBUG_MEMORY)
xmlFreeFunc xmlFree = (xmlFreeFunc) xmlMemFree;
xmlMallocFunc xmlMalloc = (xmlMallocFunc) xmlMemMalloc;
xmlMallocFunc xmlMallocAtomic = (xmlMallocFunc) xmlMemMalloc;
xmlReallocFunc xmlRealloc = (xmlReallocFunc) xmlMemRealloc;
xmlStrdupFunc xmlMemStrdup = (xmlStrdupFunc) xmlMemoryStrdup;
#else
/* [i_a] make sure MS and other platforms always do the linkage right, irrespective of whether you're using dynamic loadable of statically linked runtime libs */
static void XMLCALL lcl_xmlFreeFunc(void *mem) { free(mem); }
static void * LIBXML_ATTR_ALLOC_SIZE(1) XMLCALL lcl_xmlMallocFunc(size_t size) { return malloc(size); }
static void * XMLCALL lcl_xmlReallocFunc(void *mem, size_t size) { return realloc(mem, size); }
static char * XMLCALL lcl_xmlStrdupFunc(const char *str) { return strdup(str); }

/**
 * xmlFree:
 * @mem: an already allocated block of memory
 *
 * The variable holding the libxml free() implementation
 */
xmlFreeFunc xmlFree = lcl_xmlFreeFunc;
/**
 * xmlMalloc:
 * @size:  the size requested in bytes
 *
 * The variable holding the libxml malloc() implementation
 *
 * Returns a pointer to the newly allocated block or NULL in case of error
 */
xmlMallocFunc xmlMalloc = lcl_xmlMallocFunc;
/**
 * xmlMallocAtomic:
 * @size:  the size requested in bytes
 *
 * The variable holding the libxml malloc() implementation for atomic
 * data (i.e. blocks not containing pointers), useful when using a
 * garbage collecting allocator.
 *
 * Returns a pointer to the newly allocated block or NULL in case of error
 */
xmlMallocFunc xmlMallocAtomic = lcl_xmlMallocFunc;
/**
 * xmlRealloc:
 * @mem: an already allocated block of memory
 * @size:  the new size requested in bytes
 *
 * The variable holding the libxml realloc() implementation
 *
 * Returns a pointer to the newly reallocated block or NULL in case of error
 */
xmlReallocFunc xmlRealloc = lcl_xmlReallocFunc;
/**
 * xmlMemStrdup:
 * @str: a zero terminated string
 *
 * The variable holding the libxml strdup() implementation
 *
 * Returns the copy of the string or NULL in case of error
 */
xmlStrdupFunc xmlMemStrdup = lcl_xmlStrdupFunc;
#endif /* DEBUG_MEMORY_LOCATION || DEBUG_MEMORY */

#include "threads.h"
#include "globals.h"
#include "SAX.h"

#undef	htmlDefaultSAXHandler
#undef	oldXMLWDcompatibility
#undef	xmlBufferAllocScheme
#undef	xmlDefaultBufferSize
#undef	xmlDefaultSAXHandler
#undef	xmlDefaultSAXLocator
#undef	xmlDoValidityCheckingDefaultValue
#undef	xmlGenericError
#undef	xmlStructuredError
#undef	xmlGenericErrorContext
#undef	xmlStructuredErrorContext
#undef	xmlGetWarningsDefaultValue
#undef	xmlIndentTreeOutput
#undef  xmlTreeIndentString
#undef	xmlKeepBlanksDefaultValue
#undef	xmlLineNumbersDefaultValue
#undef	xmlLoadExtDtdDefaultValue
#undef	xmlParserDebugEntities
#undef	xmlParserVersion
#undef	xmlPedanticParserDefaultValue
#undef	xmlSaveNoEmptyTags
#undef	xmlSubstituteEntitiesDefaultValue
#undef	xmlRegisterNodeDefaultValue
#undef	xmlDeregisterNodeDefaultValue
#undef	xmlLastError

#undef  xmlParserInputBufferCreateFilenameValue
#undef  xmlOutputBufferCreateFilenameValue
/**
 * xmlParserVersion:
 *
 * Constant string describing the internal version of the library
 */
const char *xmlParserVersion = LIBXML_VERSION_STRING LIBXML_VERSION_EXTRA;

/**
 * xmlBufferAllocScheme:
 *
 * DEPRECATED: Don't use.
 *
 * Global setting, default allocation policy for buffers, default is
 * XML_BUFFER_ALLOC_EXACT
 */
xmlBufferAllocationScheme xmlBufferAllocScheme = XML_BUFFER_ALLOC_EXACT;
static xmlBufferAllocationScheme xmlBufferAllocSchemeThrDef = XML_BUFFER_ALLOC_EXACT;
/**
 * xmlDefaultBufferSize:
 *
 * DEPRECATED: Don't use.
 *
 * Global setting, default buffer size. Default value is BASE_BUFFER_SIZE
 */
int xmlDefaultBufferSize = BASE_BUFFER_SIZE;
static int xmlDefaultBufferSizeThrDef = BASE_BUFFER_SIZE;

/*
 * Parser defaults
 */

/**
 * oldXMLWDcompatibility:
 *
 * Global setting, DEPRECATED.
 */
int oldXMLWDcompatibility = 0; /* DEPRECATED */
/**
 * xmlParserDebugEntities:
 *
 * DEPRECATED: Don't use
 *
 * Global setting, asking the parser to print out debugging information.
 * while handling entities.
 * Disabled by default
 */
int xmlParserDebugEntities = 0;
static int xmlParserDebugEntitiesThrDef = 0;
/**
 * xmlDoValidityCheckingDefaultValue:
 *
 * DEPRECATED: Use the modern options API with XML_PARSE_DTDVALID.
 *
 * Global setting, indicate that the parser should work in validating mode.
 * Disabled by default.
 */
int xmlDoValidityCheckingDefaultValue = 0;
static int xmlDoValidityCheckingDefaultValueThrDef = 0;
/**
 * xmlGetWarningsDefaultValue:
 *
 * DEPRECATED: Don't use
 *
 * Global setting, indicate that the DTD validation should provide warnings.
 * Activated by default.
 */
int xmlGetWarningsDefaultValue = 1;
static int xmlGetWarningsDefaultValueThrDef = 1;
/**
 * xmlLoadExtDtdDefaultValue:
 *
 * DEPRECATED: Use the modern options API with XML_PARSE_DTDLOAD.
 *
 * Global setting, indicate that the parser should load DTD while not
 * validating.
 * Disabled by default.
 */
int xmlLoadExtDtdDefaultValue = 0;
static int xmlLoadExtDtdDefaultValueThrDef = 0;
/**
 * xmlPedanticParserDefaultValue:
 *
 * DEPRECATED: Use the modern options API with XML_PARSE_PEDANTIC.
 *
 * Global setting, indicate that the parser be pedantic
 * Disabled by default.
 */
int xmlPedanticParserDefaultValue = 0;
static int xmlPedanticParserDefaultValueThrDef = 0;
/**
 * xmlLineNumbersDefaultValue:
 *
 * DEPRECATED: The modern options API always enables line numbers.
 *
 * Global setting, indicate that the parser should store the line number
 * in the content field of elements in the DOM tree.
 * Disabled by default since this may not be safe for old classes of
 * application.
 */
int xmlLineNumbersDefaultValue = 0;
static int xmlLineNumbersDefaultValueThrDef = 0;
/**
 * xmlKeepBlanksDefaultValue:
 *
 * DEPRECATED: Use the modern options API with XML_PARSE_NOBLANKS.
 *
 * Global setting, indicate that the parser should keep all blanks
 * nodes found in the content
 * Activated by default, this is actually needed to have the parser
 * conformant to the XML Recommendation, however the option is kept
 * for some applications since this was libxml1 default behaviour.
 */
int xmlKeepBlanksDefaultValue = 1;
static int xmlKeepBlanksDefaultValueThrDef = 1;
/**
 * xmlSubstituteEntitiesDefaultValue:
 *
 * DEPRECATED: Use the modern options API with XML_PARSE_NOENT.
 *
 * Global setting, indicate that the parser should not generate entity
 * references but replace them with the actual content of the entity
 * Disabled by default, this should be activated when using XPath since
 * the XPath data model requires entities replacement and the XPath
 * engine does not handle entities references transparently.
 */
int xmlSubstituteEntitiesDefaultValue = 0;
static int xmlSubstituteEntitiesDefaultValueThrDef = 0;

/**
 * xmlRegisterNodeDefaultValue:
 *
 * DEPRECATED: Don't use
 */
xmlRegisterNodeFunc xmlRegisterNodeDefaultValue = NULL;
static xmlRegisterNodeFunc xmlRegisterNodeDefaultValueThrDef = NULL;

/**
 * xmlDeregisterNodeDefaultValue:
 *
 * DEPRECATED: Don't use
 */
xmlDeregisterNodeFunc xmlDeregisterNodeDefaultValue = NULL;
static xmlDeregisterNodeFunc xmlDeregisterNodeDefaultValueThrDef = NULL;

/**
 * xmlParserInputBufferCreateFilenameValue:
 *
 * DEPRECATED: Don't use
 */
xmlParserInputBufferCreateFilenameFunc xmlParserInputBufferCreateFilenameValue = NULL;
static xmlParserInputBufferCreateFilenameFunc xmlParserInputBufferCreateFilenameValueThrDef = NULL;

/**
 * xmlOutputBufferCreateFilenameValue:
 *
 * DEPRECATED: Don't use
 */
xmlOutputBufferCreateFilenameFunc xmlOutputBufferCreateFilenameValue = NULL;
static xmlOutputBufferCreateFilenameFunc xmlOutputBufferCreateFilenameValueThrDef = NULL;

/**
 * xmlGenericError:
 *
 * Global setting: function used for generic error callbacks
 */
xmlGenericErrorFunc xmlGenericError = xmlGenericErrorDefaultFunc;
static xmlGenericErrorFunc xmlGenericErrorThrDef = xmlGenericErrorDefaultFunc;
/**
 * xmlStructuredError:
 *
 * Global setting: function used for structured error callbacks
 */
xmlStructuredErrorFunc xmlStructuredError = NULL;
static xmlStructuredErrorFunc xmlStructuredErrorThrDef = NULL;
/**
 * xmlGenericErrorContext:
 *
 * Global setting passed to generic error callbacks
 */
void *xmlGenericErrorContext = NULL;
static void *xmlGenericErrorContextThrDef = NULL;
/**
 * xmlStructuredErrorContext:
 *
 * Global setting passed to structured error callbacks
 */
void *xmlStructuredErrorContext = NULL;
static void *xmlStructuredErrorContextThrDef = NULL;
xmlError xmlLastError;

/*
 * output defaults
 */
/**
 * xmlIndentTreeOutput:
 *
 * Global setting, asking the serializer to indent the output tree by default
 * Enabled by default
 */
int xmlIndentTreeOutput = 1;
static int xmlIndentTreeOutputThrDef = 1;

/**
 * xmlTreeIndentString:
 *
 * The string used to do one-level indent. By default is equal to "  " (two spaces)
 */
const char *xmlTreeIndentString = "  ";
static const char *xmlTreeIndentStringThrDef = "  ";

/**
 * xmlSaveNoEmptyTags:
 *
 * Global setting, asking the serializer to not output empty tags
 * as <empty/> but <empty></empty>. those two forms are indistinguishable
 * once parsed.
 * Disabled by default
 */
int xmlSaveNoEmptyTags = 0;
static int xmlSaveNoEmptyTagsThrDef = 0;

#ifdef LIBXML_SAX1_ENABLED
/**
 * xmlDefaultSAXHandler:
 *
 * DEPRECATED: This handler is unused and will be removed from future
 * versions.
 *
 * Default SAX version1 handler for XML, builds the DOM tree
 */
xmlSAXHandlerV1 xmlDefaultSAXHandler = {
    xmlSAX2InternalSubset,
    xmlSAX2IsStandalone,
    xmlSAX2HasInternalSubset,
    xmlSAX2HasExternalSubset,
    xmlSAX2ResolveEntity,
    xmlSAX2GetEntity,
    xmlSAX2EntityDecl,
    xmlSAX2NotationDecl,
    xmlSAX2AttributeDecl,
    xmlSAX2ElementDecl,
    xmlSAX2UnparsedEntityDecl,
    xmlSAX2SetDocumentLocator,
    xmlSAX2StartDocument,
    xmlSAX2EndDocument,
    xmlSAX2StartElement,
    xmlSAX2EndElement,
    xmlSAX2Reference,
    xmlSAX2Characters,
    xmlSAX2Characters,
    xmlSAX2ProcessingInstruction,
    xmlSAX2Comment,
    xmlParserWarning,
    xmlParserError,
    xmlParserError,
    xmlSAX2GetParameterEntity,
    xmlSAX2CDataBlock,
    xmlSAX2ExternalSubset,
    1,
};
#endif /* LIBXML_SAX1_ENABLED */

/**
 * xmlDefaultSAXLocator:
 *
 * DEPRECATED: Don't use
 *
 * The default SAX Locator
 * { getPublicId, getSystemId, getLineNumber, getColumnNumber}
 */
xmlSAXLocator xmlDefaultSAXLocator = {
    xmlSAX2GetPublicId,
    xmlSAX2GetSystemId,
    xmlSAX2GetLineNumber,
    xmlSAX2GetColumnNumber
};

#if defined(LIBXML_HTML_ENABLED) && defined(LIBXML_SAX1_ENABLED)
/**
 * htmlDefaultSAXHandler:
 *
 * DEPRECATED: This handler is unused and will be removed from future
 * versions.
 *
 * Default old SAX v1 handler for HTML, builds the DOM tree
 */
xmlSAXHandlerV1 htmlDefaultSAXHandler = {
    xmlSAX2InternalSubset,
    NULL,
    NULL,
    NULL,
    NULL,
    xmlSAX2GetEntity,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    xmlSAX2SetDocumentLocator,
    xmlSAX2StartDocument,
    xmlSAX2EndDocument,
    xmlSAX2StartElement,
    xmlSAX2EndElement,
    NULL,
    xmlSAX2Characters,
    xmlSAX2IgnorableWhitespace,
    xmlSAX2ProcessingInstruction,
    xmlSAX2Comment,
    xmlParserWarning,
    xmlParserError,
    xmlParserError,
    NULL,
    xmlSAX2CDataBlock,
    NULL,
    1,
};
#endif /* LIBXML_HTML_ENABLED */

/**
 * xmlInitializeGlobalState:
 * @gs: a pointer to a newly allocated global state
 *
 * xmlInitializeGlobalState() initialize a global state with all the
 * default values of the library.
 */
void
xmlInitializeGlobalState(xmlGlobalStatePtr gs)
{
#ifdef DEBUG_GLOBALS
    fprintf(stderr, "Initializing globals at %p for thread %d\n",
	    (void *) gs, xmlGetThreadId());
#endif

    xmlMutexLock(&xmlThrDefMutex);

#if defined(LIBXML_HTML_ENABLED) && defined(LIBXML_LEGACY_ENABLED) && defined(LIBXML_SAX1_ENABLED)
    inithtmlDefaultSAXHandler(&gs->htmlDefaultSAXHandler);
#endif

    gs->oldXMLWDcompatibility = 0;
    gs->xmlBufferAllocScheme = xmlBufferAllocSchemeThrDef;
    gs->xmlDefaultBufferSize = xmlDefaultBufferSizeThrDef;
#if defined(LIBXML_SAX1_ENABLED) && defined(LIBXML_LEGACY_ENABLED)
    initxmlDefaultSAXHandler(&gs->xmlDefaultSAXHandler, 1);
#endif /* LIBXML_SAX1_ENABLED */
    gs->xmlDefaultSAXLocator.getPublicId = xmlSAX2GetPublicId;
    gs->xmlDefaultSAXLocator.getSystemId = xmlSAX2GetSystemId;
    gs->xmlDefaultSAXLocator.getLineNumber = xmlSAX2GetLineNumber;
    gs->xmlDefaultSAXLocator.getColumnNumber = xmlSAX2GetColumnNumber;
    gs->xmlDoValidityCheckingDefaultValue =
         xmlDoValidityCheckingDefaultValueThrDef;
#if defined(DEBUG_MEMORY_LOCATION) | defined(DEBUG_MEMORY)
    gs->xmlFree = (xmlFreeFunc) xmlMemFree;
    gs->xmlMalloc = (xmlMallocFunc) xmlMemMalloc;
    gs->xmlMallocAtomic = (xmlMallocFunc) xmlMemMalloc;
    gs->xmlRealloc = (xmlReallocFunc) xmlMemRealloc;
    gs->xmlMemStrdup = (xmlStrdupFunc) xmlMemoryStrdup;
#else
    gs->xmlFree = (xmlFreeFunc) free;
    gs->xmlMalloc = (xmlMallocFunc) malloc;
    gs->xmlMallocAtomic = (xmlMallocFunc) malloc;
    gs->xmlRealloc = (xmlReallocFunc) realloc;
    gs->xmlMemStrdup = (xmlStrdupFunc) xmlStrdup;
#endif
    gs->xmlGetWarningsDefaultValue = xmlGetWarningsDefaultValueThrDef;
    gs->xmlIndentTreeOutput = xmlIndentTreeOutputThrDef;
    gs->xmlTreeIndentString = xmlTreeIndentStringThrDef;
    gs->xmlKeepBlanksDefaultValue = xmlKeepBlanksDefaultValueThrDef;
    gs->xmlLineNumbersDefaultValue = xmlLineNumbersDefaultValueThrDef;
    gs->xmlLoadExtDtdDefaultValue = xmlLoadExtDtdDefaultValueThrDef;
    gs->xmlParserDebugEntities = xmlParserDebugEntitiesThrDef;
    gs->xmlParserVersion = LIBXML_VERSION_STRING;
    gs->xmlPedanticParserDefaultValue = xmlPedanticParserDefaultValueThrDef;
    gs->xmlSaveNoEmptyTags = xmlSaveNoEmptyTagsThrDef;
    gs->xmlSubstituteEntitiesDefaultValue =
        xmlSubstituteEntitiesDefaultValueThrDef;

    gs->xmlGenericError = xmlGenericErrorThrDef;
    gs->xmlStructuredError = xmlStructuredErrorThrDef;
    gs->xmlGenericErrorContext = xmlGenericErrorContextThrDef;
    gs->xmlStructuredErrorContext = xmlStructuredErrorContextThrDef;
    gs->xmlRegisterNodeDefaultValue = xmlRegisterNodeDefaultValueThrDef;
    gs->xmlDeregisterNodeDefaultValue = xmlDeregisterNodeDefaultValueThrDef;

	gs->xmlParserInputBufferCreateFilenameValue = xmlParserInputBufferCreateFilenameValueThrDef;
	gs->xmlOutputBufferCreateFilenameValue = xmlOutputBufferCreateFilenameValueThrDef;
    memset(&gs->xmlLastError, 0, sizeof(xmlError));

    xmlMutexUnlock(&xmlThrDefMutex);
}

/**
 * xmlCleanupGlobals:
 *
 * DEPRECATED: This function is a no-op. Call xmlCleanupParser
 * to free global state but see the warnings there. xmlCleanupParser
 * should be only called once at program exit. In most cases, you don't
 * have call cleanup functions at all.
 */
void xmlCleanupGlobals(void) {
}

/**
 * xmlCleanupGlobalsInternal:
 *
 * Additional cleanup for multi-threading
 */
void xmlCleanupGlobalsInternal(void) {
    xmlResetError(&xmlLastError);

    xmlCleanupMutex(&xmlThrDefMutex);
    __xmlGlobalInitMutexDestroy();
}

void
xmlThrDefSetGenericErrorFunc(void *ctx, xmlGenericErrorFunc handler) {
    xmlMutexLock(&xmlThrDefMutex);
    xmlGenericErrorContextThrDef = ctx;
    if (handler != NULL)
	xmlGenericErrorThrDef = handler;
    else
	xmlGenericErrorThrDef = xmlGenericErrorDefaultFunc;
    xmlMutexUnlock(&xmlThrDefMutex);
}

void
xmlThrDefSetStructuredErrorFunc(void *ctx, xmlStructuredErrorFunc handler) {
    xmlMutexLock(&xmlThrDefMutex);
    xmlStructuredErrorContextThrDef = ctx;
    xmlStructuredErrorThrDef = handler;
    xmlMutexUnlock(&xmlThrDefMutex);
}

/**
 * xmlRegisterNodeDefault:
 * @func: function pointer to the new RegisterNodeFunc
 *
 * Registers a callback for node creation
 *
 * Returns the old value of the registration function
 */
xmlRegisterNodeFunc
xmlRegisterNodeDefault(xmlRegisterNodeFunc func)
{
    xmlRegisterNodeFunc old = xmlRegisterNodeDefaultValue;

    __xmlRegisterCallbacks = 1;
    xmlRegisterNodeDefaultValue = func;
    return(old);
}

xmlRegisterNodeFunc
xmlThrDefRegisterNodeDefault(xmlRegisterNodeFunc func)
{
    xmlRegisterNodeFunc old;

    xmlMutexLock(&xmlThrDefMutex);
    old = xmlRegisterNodeDefaultValueThrDef;

    __xmlRegisterCallbacks = 1;
    xmlRegisterNodeDefaultValueThrDef = func;
    xmlMutexUnlock(&xmlThrDefMutex);

    return(old);
}

/**
 * xmlDeregisterNodeDefault:
 * @func: function pointer to the new DeregisterNodeFunc
 *
 * Registers a callback for node destruction
 *
 * Returns the previous value of the deregistration function
 */
xmlDeregisterNodeFunc
xmlDeregisterNodeDefault(xmlDeregisterNodeFunc func)
{
    xmlDeregisterNodeFunc old = xmlDeregisterNodeDefaultValue;

    __xmlRegisterCallbacks = 1;
    xmlDeregisterNodeDefaultValue = func;
    return(old);
}

xmlDeregisterNodeFunc
xmlThrDefDeregisterNodeDefault(xmlDeregisterNodeFunc func)
{
    xmlDeregisterNodeFunc old;

    xmlMutexLock(&xmlThrDefMutex);
    old = xmlDeregisterNodeDefaultValueThrDef;

    __xmlRegisterCallbacks = 1;
    xmlDeregisterNodeDefaultValueThrDef = func;
    xmlMutexUnlock(&xmlThrDefMutex);

    return(old);
}

xmlParserInputBufferCreateFilenameFunc
xmlThrDefParserInputBufferCreateFilenameDefault(xmlParserInputBufferCreateFilenameFunc func)
{
    xmlParserInputBufferCreateFilenameFunc old;

    xmlMutexLock(&xmlThrDefMutex);
    old = xmlParserInputBufferCreateFilenameValueThrDef;
    if (old == NULL) {
		old = __xmlParserInputBufferCreateFilename;
	}

    xmlParserInputBufferCreateFilenameValueThrDef = func;
    xmlMutexUnlock(&xmlThrDefMutex);

    return(old);
}

xmlOutputBufferCreateFilenameFunc
xmlThrDefOutputBufferCreateFilenameDefault(xmlOutputBufferCreateFilenameFunc func)
{
    xmlOutputBufferCreateFilenameFunc old;

    xmlMutexLock(&xmlThrDefMutex);
    old = xmlOutputBufferCreateFilenameValueThrDef;
#ifdef LIBXML_OUTPUT_ENABLED
    if (old == NULL) {
		old = __xmlOutputBufferCreateFilename;
	}
#endif
    xmlOutputBufferCreateFilenameValueThrDef = func;
    xmlMutexUnlock(&xmlThrDefMutex);

    return(old);
}

#if defined(LIBXML_HTML_ENABLED) && defined(LIBXML_SAX1_ENABLED)
#undef	htmlDefaultSAXHandler
xmlSAXHandlerV1 *
__htmlDefaultSAXHandler(void) {
    if (IS_MAIN_THREAD)
	return (&htmlDefaultSAXHandler);
    else
	return (&xmlGetGlobalState()->htmlDefaultSAXHandler);
}
#endif

#undef xmlLastError
xmlError *
__xmlLastError(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLastError);
    else
	return (&xmlGetGlobalState()->xmlLastError);
}

/*
 * The following memory routines were apparently lost at some point,
 * and were re-inserted at this point on June 10, 2004.  Hope it's
 * the right place for them :-)
 */
#if defined(LIBXML_THREAD_ALLOC_ENABLED) && defined(LIBXML_THREAD_ENABLED)
#undef xmlMalloc
xmlMallocFunc *
__xmlMalloc(void){
    if (IS_MAIN_THREAD)
        return (&xmlMalloc);
    else
	return (&xmlGetGlobalState()->xmlMalloc);
}

#undef xmlMallocAtomic
xmlMallocFunc *
__xmlMallocAtomic(void){
    if (IS_MAIN_THREAD)
        return (&xmlMallocAtomic);
    else
        return (&xmlGetGlobalState()->xmlMallocAtomic);
}

#undef xmlRealloc
xmlReallocFunc *
__xmlRealloc(void){
    if (IS_MAIN_THREAD)
        return (&xmlRealloc);
    else
        return (&xmlGetGlobalState()->xmlRealloc);
}

#undef xmlFree
xmlFreeFunc *
__xmlFree(void){
    if (IS_MAIN_THREAD)
        return (&xmlFree);
    else
        return (&xmlGetGlobalState()->xmlFree);
}

xmlStrdupFunc *
__xmlMemStrdup(void){
    if (IS_MAIN_THREAD)
        return (&xmlMemStrdup);
    else
        return (&xmlGetGlobalState()->xmlMemStrdup);
}

#endif

/*
 * Everything starting from the line below is
 * Automatically generated by build_glob.py.
 * Do not modify the previous line.
 */


#undef	oldXMLWDcompatibility
int *
__oldXMLWDcompatibility(void) {
    if (IS_MAIN_THREAD)
	return (&oldXMLWDcompatibility);
    else
	return (&xmlGetGlobalState()->oldXMLWDcompatibility);
}

#undef	xmlBufferAllocScheme
xmlBufferAllocationScheme *
__xmlBufferAllocScheme(void) {
    if (IS_MAIN_THREAD)
	return (&xmlBufferAllocScheme);
    else
	return (&xmlGetGlobalState()->xmlBufferAllocScheme);
}
xmlBufferAllocationScheme xmlThrDefBufferAllocScheme(xmlBufferAllocationScheme v) {
    xmlBufferAllocationScheme ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlBufferAllocSchemeThrDef;
    xmlBufferAllocSchemeThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlDefaultBufferSize
int *
__xmlDefaultBufferSize(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDefaultBufferSize);
    else
	return (&xmlGetGlobalState()->xmlDefaultBufferSize);
}
int xmlThrDefDefaultBufferSize(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlDefaultBufferSizeThrDef;
    xmlDefaultBufferSizeThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#ifdef LIBXML_SAX1_ENABLED
#undef	xmlDefaultSAXHandler
xmlSAXHandlerV1 *
__xmlDefaultSAXHandler(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDefaultSAXHandler);
    else
	return (&xmlGetGlobalState()->xmlDefaultSAXHandler);
}
#endif /* LIBXML_SAX1_ENABLED */

#undef	xmlDefaultSAXLocator
xmlSAXLocator *
__xmlDefaultSAXLocator(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDefaultSAXLocator);
    else
	return (&xmlGetGlobalState()->xmlDefaultSAXLocator);
}

#undef	xmlDoValidityCheckingDefaultValue
int *
__xmlDoValidityCheckingDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDoValidityCheckingDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlDoValidityCheckingDefaultValue);
}
int xmlThrDefDoValidityCheckingDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlDoValidityCheckingDefaultValueThrDef;
    xmlDoValidityCheckingDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlGenericError
xmlGenericErrorFunc *
__xmlGenericError(void) {
    if (IS_MAIN_THREAD)
	return (&xmlGenericError);
    else
	return (&xmlGetGlobalState()->xmlGenericError);
}

#undef	xmlStructuredError
xmlStructuredErrorFunc *
__xmlStructuredError(void) {
    if (IS_MAIN_THREAD)
	return (&xmlStructuredError);
    else
	return (&xmlGetGlobalState()->xmlStructuredError);
}

#undef	xmlGenericErrorContext
void * *
__xmlGenericErrorContext(void) {
    if (IS_MAIN_THREAD)
	return (&xmlGenericErrorContext);
    else
	return (&xmlGetGlobalState()->xmlGenericErrorContext);
}

#undef	xmlStructuredErrorContext
void * *
__xmlStructuredErrorContext(void) {
    if (IS_MAIN_THREAD)
	return (&xmlStructuredErrorContext);
    else
	return (&xmlGetGlobalState()->xmlStructuredErrorContext);
}

#undef	xmlGetWarningsDefaultValue
int *
__xmlGetWarningsDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlGetWarningsDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlGetWarningsDefaultValue);
}
int xmlThrDefGetWarningsDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlGetWarningsDefaultValueThrDef;
    xmlGetWarningsDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlIndentTreeOutput
int *
__xmlIndentTreeOutput(void) {
    if (IS_MAIN_THREAD)
	return (&xmlIndentTreeOutput);
    else
	return (&xmlGetGlobalState()->xmlIndentTreeOutput);
}
int xmlThrDefIndentTreeOutput(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlIndentTreeOutputThrDef;
    xmlIndentTreeOutputThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlTreeIndentString
const char * *
__xmlTreeIndentString(void) {
    if (IS_MAIN_THREAD)
	return (&xmlTreeIndentString);
    else
	return (&xmlGetGlobalState()->xmlTreeIndentString);
}
const char * xmlThrDefTreeIndentString(const char * v) {
    const char * ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlTreeIndentStringThrDef;
    xmlTreeIndentStringThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlKeepBlanksDefaultValue
int *
__xmlKeepBlanksDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlKeepBlanksDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlKeepBlanksDefaultValue);
}
int xmlThrDefKeepBlanksDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlKeepBlanksDefaultValueThrDef;
    xmlKeepBlanksDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlLineNumbersDefaultValue
int *
__xmlLineNumbersDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLineNumbersDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlLineNumbersDefaultValue);
}
int xmlThrDefLineNumbersDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlLineNumbersDefaultValueThrDef;
    xmlLineNumbersDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlLoadExtDtdDefaultValue
int *
__xmlLoadExtDtdDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlLoadExtDtdDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlLoadExtDtdDefaultValue);
}
int xmlThrDefLoadExtDtdDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlLoadExtDtdDefaultValueThrDef;
    xmlLoadExtDtdDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlParserDebugEntities
int *
__xmlParserDebugEntities(void) {
    if (IS_MAIN_THREAD)
	return (&xmlParserDebugEntities);
    else
	return (&xmlGetGlobalState()->xmlParserDebugEntities);
}
int xmlThrDefParserDebugEntities(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlParserDebugEntitiesThrDef;
    xmlParserDebugEntitiesThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlParserVersion
const char * *
__xmlParserVersion(void) {
    if (IS_MAIN_THREAD)
	return (&xmlParserVersion);
    else
	return (&xmlGetGlobalState()->xmlParserVersion);
}

#undef	xmlPedanticParserDefaultValue
int *
__xmlPedanticParserDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlPedanticParserDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlPedanticParserDefaultValue);
}
int xmlThrDefPedanticParserDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlPedanticParserDefaultValueThrDef;
    xmlPedanticParserDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlSaveNoEmptyTags
int *
__xmlSaveNoEmptyTags(void) {
    if (IS_MAIN_THREAD)
	return (&xmlSaveNoEmptyTags);
    else
	return (&xmlGetGlobalState()->xmlSaveNoEmptyTags);
}
int xmlThrDefSaveNoEmptyTags(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlSaveNoEmptyTagsThrDef;
    xmlSaveNoEmptyTagsThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlSubstituteEntitiesDefaultValue
int *
__xmlSubstituteEntitiesDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlSubstituteEntitiesDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlSubstituteEntitiesDefaultValue);
}
int xmlThrDefSubstituteEntitiesDefaultValue(int v) {
    int ret;
    xmlMutexLock(&xmlThrDefMutex);
    ret = xmlSubstituteEntitiesDefaultValueThrDef;
    xmlSubstituteEntitiesDefaultValueThrDef = v;
    xmlMutexUnlock(&xmlThrDefMutex);
    return ret;
}

#undef	xmlRegisterNodeDefaultValue
xmlRegisterNodeFunc *
__xmlRegisterNodeDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlRegisterNodeDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlRegisterNodeDefaultValue);
}

#undef	xmlDeregisterNodeDefaultValue
xmlDeregisterNodeFunc *
__xmlDeregisterNodeDefaultValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlDeregisterNodeDefaultValue);
    else
	return (&xmlGetGlobalState()->xmlDeregisterNodeDefaultValue);
}

#undef	xmlParserInputBufferCreateFilenameValue
xmlParserInputBufferCreateFilenameFunc *
__xmlParserInputBufferCreateFilenameValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlParserInputBufferCreateFilenameValue);
    else
	return (&xmlGetGlobalState()->xmlParserInputBufferCreateFilenameValue);
}

#undef	xmlOutputBufferCreateFilenameValue
xmlOutputBufferCreateFilenameFunc *
__xmlOutputBufferCreateFilenameValue(void) {
    if (IS_MAIN_THREAD)
	return (&xmlOutputBufferCreateFilenameValue);
    else
	return (&xmlGetGlobalState()->xmlOutputBufferCreateFilenameValue);
}

