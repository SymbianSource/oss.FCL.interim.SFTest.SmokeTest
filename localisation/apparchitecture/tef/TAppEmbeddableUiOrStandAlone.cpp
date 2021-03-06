// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Supporting application for use by other test programs that need
// to query the apparc server for applications which define
// different embeddability values in their AIF files.
// 
//

/**
 @file
 @internalComponent - Internal Symbian test code 
*/

#include <coeccntx.h>

#include <apgtask.h>
#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <mw/eikmenup.h>

#include "TAppEmbedUids.h"

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

_LIT(KExampleText, "This test application defines KAppEmbeddableUiOrStandAlone in it's AIF file");


//
//
// CExampleAppView
//
//
class CExampleAppView : public CCoeControl
    {
public:
	static CExampleAppView* NewL(const TRect& aRect);
	CExampleAppView();
	~CExampleAppView();
    void ConstructL(const TRect& aRect);

private:
	// from CCoeControl
	void Draw(const TRect& /*aRect*/) const;
private:
	HBufC*  iExampleText;
    };

CExampleAppView::CExampleAppView()
	{
	}

CExampleAppView* CExampleAppView::NewL(const TRect& aRect)
	{
	CExampleAppView* self = new(ELeave) CExampleAppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}

CExampleAppView::~CExampleAppView()
	{
	delete iExampleText;
	}

void CExampleAppView::ConstructL(const TRect& aRect)
    {
	TPtrC ptr(KExampleText);
	iExampleText = ptr.AllocL();
	CreateWindowL();
	SetRect(aRect);
	ActivateL();
	}

void CExampleAppView::Draw(const TRect& /*aRect*/) const
	{
	CWindowGc& gc = SystemGc();
	TRect      drawRect = Rect();
	const CFont*     fontUsed;
	
	gc.Clear();
  	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
	TInt   baselineOffset=(drawRect.Height() - fontUsed->HeightInPixels())/2; 
	gc.DrawText(*iExampleText,drawRect,baselineOffset,CGraphicsContext::ECenter, 0);
	gc.DiscardFont();
	}


//
//
// CExampleAppUi
//
//
class CExampleAppUi : public CEikAppUi
    {
public:
    void ConstructL();
	~CExampleAppUi();

private:
    // from CEikAppUi
	void HandleCommandL(TInt aCommand);
private:
	CCoeControl* iAppView;
	};


void CExampleAppUi::ConstructL()
    {
    BaseConstructL(ENoAppResourceFile | ENoScreenFurniture);
	iAppView = CExampleAppView::NewL(ClientRect());
	}

CExampleAppUi::~CExampleAppUi()
	{
	delete iAppView;
	}

void CExampleAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EEikCmdExit: 
		Exit();
		break;
		}
	}


//
//
// CExampleDocument
//
//
class CExampleDocument : public CEikDocument
	{
public:
	static CExampleDocument* NewL(CEikApplication& aApp);
	CExampleDocument(CEikApplication& aApp);
	void ConstructL();
private: 
	           // Inherited from CEikDocument
	CEikAppUi* CreateAppUiL();
	};

CExampleDocument::CExampleDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}

CEikAppUi* CExampleDocument::CreateAppUiL()
	{
    return new(ELeave) CExampleAppUi;
	}



//
//
// CExampleApplication
//
//

class CExampleApplication : public CEikApplication
	{
private: 
	// from CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
private:
	TFileName ResourceFileName() const;
private:
	CApaDocument* CreateDocumentL(CApaProcess* a) { return CEikApplication::CreateDocumentL(a); }
	//
	};

CApaDocument* CExampleApplication::CreateDocumentL()
	{
	return new (ELeave) CExampleDocument(*this);
	}

TUid CExampleApplication::AppDllUid() const
	{
	return KUidAppEmbeddableUiOrStandAlone;
	}

TFileName CExampleApplication::ResourceFileName() const
{
	return TFileName(); // this app doesn't have a resource file
}


GLDEF_C TInt E32Dll(
					)
	{
	return KErrNone;
	}

LOCAL_C CApaApplication* NewApplication()
	{
	return new CExampleApplication;
	}

LOCAL_D const TImplementationProxy ImplementationTable[]=
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10004c4A, NewApplication)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount=sizeof(ImplementationTable)/sizeof(ImplementationTable[0]);
	return ImplementationTable;
	}

