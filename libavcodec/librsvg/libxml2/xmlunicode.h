/*
 * Summary: Unicode character APIs
 * Description: API for the Unicode character APIs
 *
 * This file is automatically generated from the
 * UCS description files of the Unicode Character Database
 * http://www.unicode.org/Public/4.0-Update1/UCD-4.0.1.html
 * using the genUnicode.py Python script.
 *
 * Generation date: Mon Mar 27 11:09:52 2006
 * Sources: Blocks-4.0.1.txt UnicodeData-4.0.1.txt
 * Author: Daniel Veillard
 */

#ifndef __XML_UNICODE_H__
#define __XML_UNICODE_H__

#include "xmlversion.h"

#ifdef LIBXML_UNICODE_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

XMLPUBFUN int xmlUCSIsAegeanNumbers	(int code);
XMLPUBFUN int xmlUCSIsAlphabeticPresentationForms	(int code);
XMLPUBFUN int xmlUCSIsArabic	(int code);
XMLPUBFUN int xmlUCSIsArabicPresentationFormsA	(int code);
XMLPUBFUN int xmlUCSIsArabicPresentationFormsB	(int code);
XMLPUBFUN int xmlUCSIsArmenian	(int code);
XMLPUBFUN int xmlUCSIsArrows	(int code);
XMLPUBFUN int xmlUCSIsBasicLatin	(int code);
XMLPUBFUN int xmlUCSIsBengali	(int code);
XMLPUBFUN int xmlUCSIsBlockElements	(int code);
XMLPUBFUN int xmlUCSIsBopomofo	(int code);
XMLPUBFUN int xmlUCSIsBopomofoExtended	(int code);
XMLPUBFUN int xmlUCSIsBoxDrawing	(int code);
XMLPUBFUN int xmlUCSIsBraillePatterns	(int code);
XMLPUBFUN int xmlUCSIsBuhid	(int code);
XMLPUBFUN int xmlUCSIsByzantineMusicalSymbols	(int code);
XMLPUBFUN int xmlUCSIsCJKCompatibility	(int code);
XMLPUBFUN int xmlUCSIsCJKCompatibilityForms	(int code);
XMLPUBFUN int xmlUCSIsCJKCompatibilityIdeographs	(int code);
XMLPUBFUN int xmlUCSIsCJKCompatibilityIdeographsSupplement	(int code);
XMLPUBFUN int xmlUCSIsCJKRadicalsSupplement	(int code);
XMLPUBFUN int xmlUCSIsCJKSymbolsandPunctuation	(int code);
XMLPUBFUN int xmlUCSIsCJKUnifiedIdeographs	(int code);
XMLPUBFUN int xmlUCSIsCJKUnifiedIdeographsExtensionA	(int code);
XMLPUBFUN int xmlUCSIsCJKUnifiedIdeographsExtensionB	(int code);
XMLPUBFUN int xmlUCSIsCherokee	(int code);
XMLPUBFUN int xmlUCSIsCombiningDiacriticalMarks	(int code);
XMLPUBFUN int xmlUCSIsCombiningDiacriticalMarksforSymbols	(int code);
XMLPUBFUN int xmlUCSIsCombiningHalfMarks	(int code);
XMLPUBFUN int xmlUCSIsCombiningMarksforSymbols	(int code);
XMLPUBFUN int xmlUCSIsControlPictures	(int code);
XMLPUBFUN int xmlUCSIsCurrencySymbols	(int code);
XMLPUBFUN int xmlUCSIsCypriotSyllabary	(int code);
XMLPUBFUN int xmlUCSIsCyrillic	(int code);
XMLPUBFUN int xmlUCSIsCyrillicSupplement	(int code);
XMLPUBFUN int xmlUCSIsDeseret	(int code);
XMLPUBFUN int xmlUCSIsDevanagari	(int code);
XMLPUBFUN int xmlUCSIsDingbats	(int code);
XMLPUBFUN int xmlUCSIsEnclosedAlphanumerics	(int code);
XMLPUBFUN int xmlUCSIsEnclosedCJKLettersandMonths	(int code);
XMLPUBFUN int xmlUCSIsEthiopic	(int code);
XMLPUBFUN int xmlUCSIsGeneralPunctuation	(int code);
XMLPUBFUN int xmlUCSIsGeometricShapes	(int code);
XMLPUBFUN int xmlUCSIsGeorgian	(int code);
XMLPUBFUN int xmlUCSIsGothic	(int code);
XMLPUBFUN int xmlUCSIsGreek	(int code);
XMLPUBFUN int xmlUCSIsGreekExtended	(int code);
XMLPUBFUN int xmlUCSIsGreekandCoptic	(int code);
XMLPUBFUN int xmlUCSIsGujarati	(int code);
XMLPUBFUN int xmlUCSIsGurmukhi	(int code);
XMLPUBFUN int xmlUCSIsHalfwidthandFullwidthForms	(int code);
XMLPUBFUN int xmlUCSIsHangulCompatibilityJamo	(int code);
XMLPUBFUN int xmlUCSIsHangulJamo	(int code);
XMLPUBFUN int xmlUCSIsHangulSyllables	(int code);
XMLPUBFUN int xmlUCSIsHanunoo	(int code);
XMLPUBFUN int xmlUCSIsHebrew	(int code);
XMLPUBFUN int xmlUCSIsHighPrivateUseSurrogates	(int code);
XMLPUBFUN int xmlUCSIsHighSurrogates	(int code);
XMLPUBFUN int xmlUCSIsHiragana	(int code);
XMLPUBFUN int xmlUCSIsIPAExtensions	(int code);
XMLPUBFUN int xmlUCSIsIdeographicDescriptionCharacters	(int code);
XMLPUBFUN int xmlUCSIsKanbun	(int code);
XMLPUBFUN int xmlUCSIsKangxiRadicals	(int code);
XMLPUBFUN int xmlUCSIsKannada	(int code);
XMLPUBFUN int xmlUCSIsKatakana	(int code);
XMLPUBFUN int xmlUCSIsKatakanaPhoneticExtensions	(int code);
XMLPUBFUN int xmlUCSIsKhmer	(int code);
XMLPUBFUN int xmlUCSIsKhmerSymbols	(int code);
XMLPUBFUN int xmlUCSIsLao	(int code);
XMLPUBFUN int xmlUCSIsLatin1Supplement	(int code);
XMLPUBFUN int xmlUCSIsLatinExtendedA	(int code);
XMLPUBFUN int xmlUCSIsLatinExtendedB	(int code);
XMLPUBFUN int xmlUCSIsLatinExtendedAdditional	(int code);
XMLPUBFUN int xmlUCSIsLetterlikeSymbols	(int code);
XMLPUBFUN int xmlUCSIsLimbu	(int code);
XMLPUBFUN int xmlUCSIsLinearBIdeograms	(int code);
XMLPUBFUN int xmlUCSIsLinearBSyllabary	(int code);
XMLPUBFUN int xmlUCSIsLowSurrogates	(int code);
XMLPUBFUN int xmlUCSIsMalayalam	(int code);
XMLPUBFUN int xmlUCSIsMathematicalAlphanumericSymbols	(int code);
XMLPUBFUN int xmlUCSIsMathematicalOperators	(int code);
XMLPUBFUN int xmlUCSIsMiscellaneousMathematicalSymbolsA	(int code);
XMLPUBFUN int xmlUCSIsMiscellaneousMathematicalSymbolsB	(int code);
XMLPUBFUN int xmlUCSIsMiscellaneousSymbols	(int code);
XMLPUBFUN int xmlUCSIsMiscellaneousSymbolsandArrows	(int code);
XMLPUBFUN int xmlUCSIsMiscellaneousTechnical	(int code);
XMLPUBFUN int xmlUCSIsMongolian	(int code);
XMLPUBFUN int xmlUCSIsMusicalSymbols	(int code);
XMLPUBFUN int xmlUCSIsMyanmar	(int code);
XMLPUBFUN int xmlUCSIsNumberForms	(int code);
XMLPUBFUN int xmlUCSIsOgham	(int code);
XMLPUBFUN int xmlUCSIsOldItalic	(int code);
XMLPUBFUN int xmlUCSIsOpticalCharacterRecognition	(int code);
XMLPUBFUN int xmlUCSIsOriya	(int code);
XMLPUBFUN int xmlUCSIsOsmanya	(int code);
XMLPUBFUN int xmlUCSIsPhoneticExtensions	(int code);
XMLPUBFUN int xmlUCSIsPrivateUse	(int code);
XMLPUBFUN int xmlUCSIsPrivateUseArea	(int code);
XMLPUBFUN int xmlUCSIsRunic	(int code);
XMLPUBFUN int xmlUCSIsShavian	(int code);
XMLPUBFUN int xmlUCSIsSinhala	(int code);
XMLPUBFUN int xmlUCSIsSmallFormVariants	(int code);
XMLPUBFUN int xmlUCSIsSpacingModifierLetters	(int code);
XMLPUBFUN int xmlUCSIsSpecials	(int code);
XMLPUBFUN int xmlUCSIsSuperscriptsandSubscripts	(int code);
XMLPUBFUN int xmlUCSIsSupplementalArrowsA	(int code);
XMLPUBFUN int xmlUCSIsSupplementalArrowsB	(int code);
XMLPUBFUN int xmlUCSIsSupplementalMathematicalOperators	(int code);
XMLPUBFUN int xmlUCSIsSupplementaryPrivateUseAreaA	(int code);
XMLPUBFUN int xmlUCSIsSupplementaryPrivateUseAreaB	(int code);
XMLPUBFUN int xmlUCSIsSyriac	(int code);
XMLPUBFUN int xmlUCSIsTagalog	(int code);
XMLPUBFUN int xmlUCSIsTagbanwa	(int code);
XMLPUBFUN int xmlUCSIsTags	(int code);
XMLPUBFUN int xmlUCSIsTaiLe	(int code);
XMLPUBFUN int xmlUCSIsTaiXuanJingSymbols	(int code);
XMLPUBFUN int xmlUCSIsTamil	(int code);
XMLPUBFUN int xmlUCSIsTelugu	(int code);
XMLPUBFUN int xmlUCSIsThaana	(int code);
XMLPUBFUN int xmlUCSIsThai	(int code);
XMLPUBFUN int xmlUCSIsTibetan	(int code);
XMLPUBFUN int xmlUCSIsUgaritic	(int code);
XMLPUBFUN int xmlUCSIsUnifiedCanadianAboriginalSyllabics	(int code);
XMLPUBFUN int xmlUCSIsVariationSelectors	(int code);
XMLPUBFUN int xmlUCSIsVariationSelectorsSupplement	(int code);
XMLPUBFUN int xmlUCSIsYiRadicals	(int code);
XMLPUBFUN int xmlUCSIsYiSyllables	(int code);
XMLPUBFUN int xmlUCSIsYijingHexagramSymbols	(int code);

XMLPUBFUN int xmlUCSIsBlock	(int code, const char *block);

XMLPUBFUN int xmlUCSIsCatC	(int code);
XMLPUBFUN int xmlUCSIsCatCc	(int code);
XMLPUBFUN int xmlUCSIsCatCf	(int code);
XMLPUBFUN int xmlUCSIsCatCo	(int code);
XMLPUBFUN int xmlUCSIsCatCs	(int code);
XMLPUBFUN int xmlUCSIsCatL	(int code);
XMLPUBFUN int xmlUCSIsCatLl	(int code);
XMLPUBFUN int xmlUCSIsCatLm	(int code);
XMLPUBFUN int xmlUCSIsCatLo	(int code);
XMLPUBFUN int xmlUCSIsCatLt	(int code);
XMLPUBFUN int xmlUCSIsCatLu	(int code);
XMLPUBFUN int xmlUCSIsCatM	(int code);
XMLPUBFUN int xmlUCSIsCatMc	(int code);
XMLPUBFUN int xmlUCSIsCatMe	(int code);
XMLPUBFUN int xmlUCSIsCatMn	(int code);
XMLPUBFUN int xmlUCSIsCatN	(int code);
XMLPUBFUN int xmlUCSIsCatNd	(int code);
XMLPUBFUN int xmlUCSIsCatNl	(int code);
XMLPUBFUN int xmlUCSIsCatNo	(int code);
XMLPUBFUN int xmlUCSIsCatP	(int code);
XMLPUBFUN int xmlUCSIsCatPc	(int code);
XMLPUBFUN int xmlUCSIsCatPd	(int code);
XMLPUBFUN int xmlUCSIsCatPe	(int code);
XMLPUBFUN int xmlUCSIsCatPf	(int code);
XMLPUBFUN int xmlUCSIsCatPi	(int code);
XMLPUBFUN int xmlUCSIsCatPo	(int code);
XMLPUBFUN int xmlUCSIsCatPs	(int code);
XMLPUBFUN int xmlUCSIsCatS	(int code);
XMLPUBFUN int xmlUCSIsCatSc	(int code);
XMLPUBFUN int xmlUCSIsCatSk	(int code);
XMLPUBFUN int xmlUCSIsCatSm	(int code);
XMLPUBFUN int xmlUCSIsCatSo	(int code);
XMLPUBFUN int xmlUCSIsCatZ	(int code);
XMLPUBFUN int xmlUCSIsCatZl	(int code);
XMLPUBFUN int xmlUCSIsCatZp	(int code);
XMLPUBFUN int xmlUCSIsCatZs	(int code);

XMLPUBFUN int xmlUCSIsCat	(int code, const char *cat);

#ifdef __cplusplus
}
#endif

#endif /* LIBXML_UNICODE_ENABLED */

#endif /* __XML_UNICODE_H__ */
