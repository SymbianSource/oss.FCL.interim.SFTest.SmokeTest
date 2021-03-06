// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the header file for CT_MsgCheckSmtpSNAPSetting class
// 
//

#ifndef T_CHECK_SMTP_SNAP_SETTING_H
#define T_CHECK_SMTP_SNAP_SETTING_H

/* User include */
#include "T_MsgSyncStepSMTP.h"

/* Literal used */
_LIT(KCheckSmtpSNAPSetting,"CheckSmtpSNAPSetting");

/* Implements a test step to check the SMTP SNAP information for the account */
class CT_MsgCheckSmtpSNAPSetting : public CT_MsgSyncStepSMTP
	{
public:
	CT_MsgCheckSmtpSNAPSetting(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	TBool ReadIni();
	
private:
	TPtrC iSmtpAccountName;
	TBool iExpectedSNAPDefinition;
	
	};
	
#endif /* T_CHECK_SMTP_SNAP_SETTING_H */
