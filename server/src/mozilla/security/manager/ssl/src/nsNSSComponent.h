/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 *  Version: GPL 3
 * 
 *  This program is licensed under the GNU General Public license, version 3 (GPL).
 *  It is derived from Mozilla software and modified by Aptana, Inc.
 *  Aptana, Inc. has elected to use and license the Mozilla software 
 *  under the terms of the GPL, and licenses this file to you under the terms
 *  of the GPL.
 *  
 *  Contributor(s): Aptana, Inc.
 *  The portions modified by Aptana are Copyright (C) 2007-2008 Aptana, Inc.
 *  All Rights Reserved.
 * 
 *  This program is distributed in the hope that it will be useful, but
 *  AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 *  NONINFRINGEMENT. Redistribution, except as permitted by the GPL,
 *  is prohibited.
 * 
 *  You can redistribute and/or modify this program under the terms of the GPL, 
 *  as published by the Free Software Foundation.  You should
 *  have received a copy of the GNU General Public License, Version 3 along
 *  with this program; if not, write to the Free Software Foundation, Inc., 51
 *  Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *  
 *  Aptana provides a special exception to allow redistribution of this file
 *  with certain other code and certain additional terms
 *  pursuant to Section 7 of the GPL. You may view the exception and these
 *  terms on the web at http://www.aptana.com/legal/gpl/.
 *  
 *  You may view the GPL, and Aptana's exception and additional terms in the file
 *  titled license-jaxer.html in the main distribution folder of this program.
 *  
 *  Any modifications to this file must keep this entire header intact.
 * 
 * ***** END LICENSE BLOCK ***** */
/* ***** BEGIN ORIGINAL ATTRIBUTION BLOCK *****
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Hubbie Shaw
 *   Doug Turner <dougt@netscape.com>
 *   Brian Ryner <bryner@brianryner.com>
 *   Kai Engert <kaie@netscape.com>
 *   Kai Engert <kengert@redhat.com>
 *
 * ***** END ORIGINAL ATTRIBUTION BLOCK ***** */

#ifndef _nsNSSComponent_h_
#define _nsNSSComponent_h_

#include "nsCOMPtr.h"
#include "nsISignatureVerifier.h"
#include "nsIURIContentListener.h"
#include "nsIStreamListener.h"
#include "nsIEntropyCollector.h"
#include "nsString.h"
#include "nsIStringBundle.h"
#include "nsIDOMEventTarget.h"
#include "nsIPrefBranch.h"
#include "nsIObserver.h"
#include "nsIObserverService.h"
#include "nsWeakReference.h"
#include "nsIScriptSecurityManager.h"
#include "nsSmartCardMonitor.h"
#include "nsINSSErrorsService.h"
#include "nsITimer.h"
#include "nsNetUtil.h"
#include "nsHashtable.h"
#include "prlock.h"
#include "nsICryptoHash.h"
#include "nsICryptoHMAC.h"
#include "hasht.h"
#include "nsNSSCallbacks.h"

#include "nsNSSHelper.h"

#define NS_NSSCOMPONENT_CID \
{0xa277189c, 0x1dd1, 0x11b2, {0xa8, 0xc9, 0xe4, 0xe8, 0xbf, 0xb1, 0x33, 0x8e}}

#define PSM_COMPONENT_CONTRACTID "@mozilla.org/psm;1"
#define PSM_COMPONENT_CLASSNAME "Mozilla PSM Component"

//Define an interface that we can use to look up from the
//callbacks passed to NSS.

#define NS_INSSCOMPONENT_IID_STR "d4b49dd6-1dd1-11b2-b6fe-b14cfaf69cbd"
#define NS_INSSCOMPONENT_IID \
  {0xd4b49dd6, 0x1dd1, 0x11b2, \
    { 0xb6, 0xfe, 0xb1, 0x4c, 0xfa, 0xf6, 0x9c, 0xbd }}

#define NS_PSMCONTENTLISTEN_CID {0xc94f4a30, 0x64d7, 0x11d4, {0x99, 0x60, 0x00, 0xb0, 0xd0, 0x23, 0x54, 0xa0}}
#define NS_PSMCONTENTLISTEN_CONTRACTID "@mozilla.org/security/psmdownload;1"

#define NS_CRYPTO_HASH_CLASSNAME "Mozilla Crypto Hash Function Component"
#define NS_CRYPTO_HASH_CID {0x36a1d3b3, 0xd886, 0x4317, {0x96, 0xff, 0x87, 0xb0, 0x00, 0x5c, 0xfe, 0xf7}}

#define NS_CRYPTO_HMAC_CLASSNAME "Mozilla Crypto HMAC Function Component"
#define NS_CRYPTO_HMAC_CID {0xa496d0a2, 0xdff7, 0x4e23, {0xbd, 0x65, 0x1c, 0xa7, 0x42, 0xfa, 0x17, 0x8a}}

//--------------------------------------------
// Now we need a content listener to register 
//--------------------------------------------
class PSMContentDownloader : public nsIStreamListener
{
public:
  PSMContentDownloader() {NS_ASSERTION(PR_FALSE, "don't use this constructor."); }
  PSMContentDownloader(PRUint32 type);
  virtual ~PSMContentDownloader();
  void setSilentDownload(PRBool flag);
  void setCrlAutodownloadKey(nsAutoString key);

  NS_DECL_ISUPPORTS
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER

  enum {UNKNOWN_TYPE = 0};
  enum {X509_CA_CERT  = 1};
  enum {X509_USER_CERT  = 2};
  enum {X509_EMAIL_CERT  = 3};
  enum {X509_SERVER_CERT  = 4};
  enum {PKCS7_CRL = 5};

protected:
  char* mByteData;
  PRInt32 mBufferOffset;
  PRInt32 mBufferSize;
  PRUint32 mType;
  PRBool mDoSilentDownload;
  nsAutoString mCrlAutoDownloadKey;
  nsCOMPtr<nsIURI> mURI;
  nsresult handleContentDownloadError(nsresult errCode);
};

class NS_NO_VTABLE nsINSSComponent : public nsISupports {
 public:
  NS_DECLARE_STATIC_IID_ACCESSOR(NS_INSSCOMPONENT_IID)

  NS_IMETHOD GetPIPNSSBundleString(const char *name,
                                   nsAString &outString) = 0;
  NS_IMETHOD PIPBundleFormatStringFromName(const char *name,
                                           const PRUnichar **params,
                                           PRUint32 numParams,
                                           nsAString &outString) = 0;

  NS_IMETHOD GetNSSBundleString(const char *name,
                                nsAString &outString) = 0;
  NS_IMETHOD NSSBundleFormatStringFromName(const char *name,
                                           const PRUnichar **params,
                                           PRUint32 numParams,
                                           nsAString &outString) = 0;

  // This method will just disable OCSP in NSS, it will not
  // alter the respective pref values.
  NS_IMETHOD SkipOcsp() = 0;

  // This method will set the OCSP value according to the 
  // values in the preferences.
  NS_IMETHOD SkipOcspOff() = 0;

  NS_IMETHOD RememberCert(CERTCertificate *cert) = 0;

  NS_IMETHOD RemoveCrlFromList(nsAutoString) = 0;

  NS_IMETHOD DefineNextTimer() = 0;

  NS_IMETHOD DownloadCRLDirectly(nsAutoString, nsAutoString) = 0;
  
  NS_IMETHOD LogoutAuthenticatedPK11() = 0;

  NS_IMETHOD LaunchSmartCardThread(SECMODModule *module) = 0;

  NS_IMETHOD ShutdownSmartCardThread(SECMODModule *module) = 0;

  NS_IMETHOD PostEvent(const nsAString &eventType, const nsAString &token) = 0;

  NS_IMETHOD DispatchEvent(const nsAString &eventType, const nsAString &token) = 0;
  
  NS_IMETHOD EnsureIdentityInfoLoaded() = 0;
};

NS_DEFINE_STATIC_IID_ACCESSOR(nsINSSComponent, NS_INSSCOMPONENT_IID)

class nsCryptoHash : public nsICryptoHash
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICRYPTOHASH

  nsCryptoHash();

private:
  ~nsCryptoHash();
  HASHContext* mHashContext;
};

class nsCryptoHMAC : public nsICryptoHMAC
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICRYPTOHMAC

  nsCryptoHMAC();

private:
  ~nsCryptoHMAC();

  PK11Context* mHMACContext;
};

struct PRLock;
class nsNSSShutDownList;
class nsSSLThread;
class nsCertVerificationThread;

// Implementation of the PSM component interface.
class nsNSSComponent : public nsISignatureVerifier,
                       public nsIEntropyCollector,
                       public nsINSSComponent,
                       public nsIObserver,
                       public nsSupportsWeakReference,
                       public nsITimerCallback,
                       public nsINSSErrorsService
{
public:
  NS_DEFINE_STATIC_CID_ACCESSOR( NS_NSSCOMPONENT_CID )

  nsNSSComponent();
  virtual ~nsNSSComponent();

  NS_DECL_ISUPPORTS
  NS_DECL_NSISIGNATUREVERIFIER
  NS_DECL_NSIENTROPYCOLLECTOR
  NS_DECL_NSIOBSERVER
  NS_DECL_NSITIMERCALLBACK
  NS_DECL_NSINSSERRORSSERVICE

  NS_METHOD Init();

  NS_IMETHOD GetPIPNSSBundleString(const char *name,
                                   nsAString &outString);
  NS_IMETHOD PIPBundleFormatStringFromName(const char *name,
                                           const PRUnichar **params,
                                           PRUint32 numParams,
                                           nsAString &outString);
  NS_IMETHOD GetNSSBundleString(const char *name,
                               nsAString &outString);
  NS_IMETHOD NSSBundleFormatStringFromName(const char *name,
                                           const PRUnichar **params,
                                           PRUint32 numParams,
                                           nsAString &outString);
  NS_IMETHOD SkipOcsp();
  NS_IMETHOD SkipOcspOff();
  nsresult InitializeCRLUpdateTimer();
  nsresult StopCRLUpdateTimer();
  NS_IMETHOD RemoveCrlFromList(nsAutoString);
  NS_IMETHOD DefineNextTimer();
  NS_IMETHOD LogoutAuthenticatedPK11();
  NS_IMETHOD DownloadCRLDirectly(nsAutoString, nsAutoString);
  NS_IMETHOD RememberCert(CERTCertificate *cert);
  static nsresult GetNSSCipherIDFromPrefString(const nsACString &aPrefString, PRUint16 &aCipherId);

  NS_IMETHOD LaunchSmartCardThread(SECMODModule *module);
  NS_IMETHOD ShutdownSmartCardThread(SECMODModule *module);
  NS_IMETHOD PostEvent(const nsAString &eventType, const nsAString &token);
  NS_IMETHOD DispatchEvent(const nsAString &eventType, const nsAString &token);
  NS_IMETHOD EnsureIdentityInfoLoaded();

private:

  nsresult InitializeNSS(PRBool showWarningBox);
  nsresult ShutdownNSS();

#ifdef XP_MACOSX
  void TryCFM2MachOMigration(nsIFile *cfmPath, nsIFile *machoPath);
#endif
  
  enum AlertIdentifier {
    ai_nss_init_problem, 
    ai_sockets_still_active, 
    ai_crypto_ui_active,
    ai_incomplete_logout
  };
  
  void ShowAlert(AlertIdentifier ai);
  void InstallLoadableRoots();
  void UnloadLoadableRoots();
  void LaunchSmartCardThreads();
  void ShutdownSmartCardThreads();
  void CleanupIdentityInfo();
  nsresult InitializePIPNSSBundle();
  nsresult ConfigureInternalPKCS11Token();
  nsresult RegisterPSMContentListener();
  nsresult RegisterObservers();
  nsresult DownloadCrlSilently();
  nsresult PostCRLImportEvent(const nsCSubstring &urlString, nsIStreamListener *psmDownloader);
  nsresult getParamsForNextCrlToDownload(nsAutoString *url, PRTime *time, nsAutoString *key);
  nsresult DispatchEventToWindow(nsIDOMWindow *domWin, const nsAString &eventType, const nsAString &token);

#ifndef JAXER
  // Methods that we use to handle the profile change notifications (and to
  // synthesize a full profile change when we're just doing a profile startup):
  void DoProfileApproveChange(nsISupports* aSubject);
  void DoProfileChangeNetTeardown();
  void DoProfileChangeTeardown(nsISupports* aSubject);
  void DoProfileBeforeChange(nsISupports* aSubject);
  void DoProfileChangeNetRestore();
#endif /* JAXER */

  PRLock *mutex;
  
  nsCOMPtr<nsIScriptSecurityManager> mScriptSecurityManager;
  nsCOMPtr<nsIStringBundle> mPIPNSSBundle;
  nsCOMPtr<nsIStringBundle> mNSSErrorsBundle;
  nsCOMPtr<nsIURIContentListener> mPSMContentListener;
  nsCOMPtr<nsIPrefBranch> mPrefBranch;
  nsCOMPtr<nsITimer> mTimer;
  PRBool mNSSInitialized;
  PRBool mObserversRegistered;
  PLHashTable *hashTableCerts;
  nsAutoString mDownloadURL;
  nsAutoString mCrlUpdateKey;
  PRLock *mCrlTimerLock;
  nsHashtable *crlsScheduledForDownload;
  PRBool crlDownloadTimerOn;
  PRBool mUpdateTimerInitialized;
  static int mInstanceCount;
  nsNSSShutDownList *mShutdownObjectList;
  SmartCardThreadList *mThreadList;
  PRBool mIsNetworkDown;
  nsSSLThread *mSSLThread;
  nsCertVerificationThread *mCertVerificationThread;
  nsNSSHttpInterface mHttpForNSS;

  static PRStatus PR_CALLBACK IdentityInfoInit(void);
  PRCallOnceType mIdentityInfoCallOnce;
};

class PSMContentListener : public nsIURIContentListener,
                            public nsSupportsWeakReference {
public:
  PSMContentListener();
  virtual ~PSMContentListener();
  nsresult init();

  NS_DECL_ISUPPORTS
  NS_DECL_NSIURICONTENTLISTENER
private:
  nsCOMPtr<nsISupports> mLoadCookie;
  nsCOMPtr<nsIURIContentListener> mParentContentListener;
};

class nsNSSErrors
{
public:
  static const char *getDefaultErrorStringName(PRInt32 err);
  static const char *getOverrideErrorStringName(PRInt32 aErrorCode);
  static nsresult getErrorMessageFromCode(PRInt32 err,
                                          nsINSSComponent *component,
                                          nsString &returnedMessage);
};

#endif // _nsNSSComponent_h_
