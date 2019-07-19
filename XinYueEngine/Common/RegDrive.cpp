
#include "RegDrive.h"


RegDrive::RegDrive()
{

}


 


BOOL RegDrive::GetRegStr(tstring strKeyName,
	tstring strValueName,
	tstring& strValue,
	DWORD dwMaxLength,
	HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	dwMaxLength++;
	TCHAR* pBuf = new TCHAR[dwMaxLength];
	lRet = rk.QueryStringValue(strValueName.c_str(), pBuf, &dwMaxLength);
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	else
	{
		strValue = tstring(pBuf);
	}
	rk.Close();
	delete[] pBuf;
	return bRet;
}

BOOL RegDrive::GetRegDWORD(tstring strKeyName, tstring strValueName, DWORD& dwValue, HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	lRet = rk.QueryDWORDValue(strValueName.c_str(), dwValue);
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	rk.Close();
	return bRet;
}

BOOL RegDrive::GetRegBinary(tstring strKeyName,
	tstring strValueName,
	BYTE* pBuf,
	DWORD& dwBytes,
	HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	lRet = rk.QueryBinaryValue(strValueName.c_str(), pBuf, &dwBytes);
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	rk.Close();
	return bRet;
}

BOOL RegDrive::SetRegStr(tstring strKeyName, tstring strValueName, tstring strValue, HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	lRet = rk.SetStringValue(strValueName.c_str(), strValue.c_str());
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	rk.Close();
	return bRet;
}

BOOL RegDrive::SetRegDWORD(tstring strKeyName, tstring strValueName, DWORD dwValue, HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	lRet = rk.SetDWORDValue(strValueName.c_str(), dwValue);
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	rk.Close();
	return bRet;
}

BOOL RegDrive::SetRegBinary(tstring strKeyName,
	tstring strValueName,
	BYTE* pBuf,
	DWORD dwBytes,
	HKEY hKeyParent)
{
	BOOL bRet = TRUE;
	CRegKey rk;
	LONG lRet = rk.Open(hKeyParent, strKeyName.c_str());
	if (lRet != ERROR_SUCCESS)
		lRet = rk.Create(hKeyParent, strKeyName.c_str());

	lRet = rk.SetBinaryValue(strValueName.c_str(), pBuf, dwBytes);
	if (lRet != ERROR_SUCCESS)
	{
		bRet = FALSE;
	}
	rk.Close();
	return bRet;
}

BOOL RegDrive::DelRegSubKey(tstring strKeyName, tstring strSubKey, HKEY hKeyParent, bool bRecurse)
{
	CRegKey rk;
	if (rk.Open(hKeyParent, strKeyName.c_str()) == ERROR_SUCCESS)
	{
		LONG lRet;
		if (bRecurse)
			lRet = rk.RecurseDeleteKey(strSubKey.c_str());
		else
			lRet = rk.DeleteSubKey(strSubKey.c_str());
		rk.Close();
		return lRet == ERROR_SUCCESS;
	}
	else
	{
		return FALSE;
	}
}

BOOL RegDrive::DelRegValue(tstring strKeyName, tstring strValueName, HKEY hKeyParent)
{
	CRegKey rk;
	if (rk.Open(hKeyParent, strKeyName.c_str()) == ERROR_SUCCESS)
	{
		rk.DeleteValue(strValueName.c_str());
		rk.Close();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
