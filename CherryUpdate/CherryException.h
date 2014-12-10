#pragma once

#ifndef _CHERRY_EXCEPTION_H
#define _CHERRY_EXCEPTION_H

///////////////////////////////////////////////////////////////////////////
///
/// \class		CCherryException
/// \brief		예외 enum을 나열한 wrapper 클래스
/// \author		오수철
/// \date		2013-09-24
/// \remark		
/// \section	
///
///////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CCherryException
{
public:
	enum CHERRY_ERROR
	{
		// General
		ERROR_CHERRY_SUCCESS,
		ERROR_CHERRY_FAILURE,

		// CherryUpdate
		ERROR_UPDATE_INCOMING_DIR_CREATE_FAIL,
		ERROR_UPDATE_RECEIVE_PROFILE_FAIL,
		ERROR_UPDATE_NO_CONTEXT_PROFILE,
		ERROR_UPDATE_PROFILE_XML_PARSE_FAIL,
		ERROR_UPDATE_PROFILE_XML_NO_SERVER_VERSION,
		ERROR_UPDATE_PROFILE_XML_NO_CLIENT_VERSION,
		ERROR_UPDATE_NO_UPDATE_IS_REQUIRED,
		

	};
};

typedef CCherryException::CHERRY_ERROR CHERRY_RET;

#endif