// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//



/**
 @file
 @test
 @internalComponent - Internal Symbian test code 
*/


#if (!defined __APPARTESTSERVER_H__)
#define __APPARTESTSERVER_H__

#include <TestExecuteStepBase.h>
#include <TestExecuteServerBase.h>
#include <testexecutelog.h>

class CApparctestServer : public CTestServer
	{
public:
	static CApparctestServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};

#endif
