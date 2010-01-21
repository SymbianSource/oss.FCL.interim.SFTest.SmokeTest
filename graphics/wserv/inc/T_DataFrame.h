/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#if (!defined __T_GRAPHICS_WSERV_FRAME_H__)
#define __T_GRAPHICS_WSERV_FRAME_H__

#include <w32stdgraphic.h>

/**
 * Test Active Notification class
 *
 */
class CT_DataFrame : public CDataWrapperBase
	{
public:
	/**
	* Two phase constructor
	*/
	static CT_DataFrame*	NewL();

	/**
	* Public destructor
	*/
	~CT_DataFrame();

	/**
	* Return a pointer to the object that the data wraps
	*
	* \return pointer to the object that the data wraps
	*/
	virtual TAny*	GetObject()	{ return iFrame; }

	/**
	* Set the object that the data wraps
	*
	* @param	aObject object that the wrapper is testing
	*
	*/
	virtual void	SetObjectL(TAny* aAny);

	/**
	* The object will no longer be owned by this
	*
	* @leave	KErrNotSupported if the the function is not supported
	*/
	virtual void	DisownObjectL();
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	/**
	* Protected constructor. First phase construction
	*/
	CT_DataFrame();

	/**
	* Second phase construction
	*/
	void  ConstructL();

private:
	void	DoCmdDestroy();
    void	DoCmdNewL();
	void    DoCmdFrameInfo(const TDesC& aSection);
	void	DoCmdSetFrameInfo(const TDesC& aSection);
    void    DoCmdBitmapL(const TDesC& aSection);
    void    DoCmdSetBitmapL(const TDesC& aSection);
	void    DoCmdMaskL(const TDesC& aSection);
    void    DoCmdSetMaskL(const TDesC& aSection);
private:
    CWsGraphicBitmapAnimation::CFrame* iFrame;
	};

#endif /* __T_GRAPHICS_WSERV_FRAME_H__ */