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
//

/**
 @file
 @internalComponent - Internal Symbian test code 
*/
 

#if (!defined __T_groupNametest_H__)
#define __T_groupNametest_H__

#include "apparctestserver.h"

//  A CT_GroupNameStep test class. 

class CT_GroupNameStep : public CTestStep
	{
public:
	CT_GroupNameStep();
	~CT_GroupNameStep();
	virtual TVerdict doTestStepL();
	void DoTestGroupNameL(RApaLsSession& aLs);

private:
	};


_LIT(KT_GroupNameStep, "T_GroupName");

#endif

