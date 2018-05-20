
#ifdef __BORLANDC__


#ifndef __BCBEXT_H__
#define __BCBEXT_H__



//***************************************************************
// often used style is:
// Editbox: ES_NUMBER
// RadioButton:BS_PUSHLIKE

inline int SetCtrlStyle(TWinControl * ctl, DWORD Style)
{
	//TODO: Add your source code here
    HWND hWnd = ctl->Handle;
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle | Style);
	return ::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
}



//***************************************************************
// the following three function are used to make conversion between
// float and currency string type.

inline AnsiString FloatToThousandStr(double value)
{
	return FormatFloat("###,###,###,##0.00",value);
}

inline AnsiString AddThousandChar(AnsiString s)
{
	String ts = FloatToThousandStr(s.ToDouble());
    s = ts;
	return s;
}

inline AnsiString RemoveThousandChar(AnsiString& s)
{
	while( s.Pos(",") )  s.Delete( s.Pos(","),1 );
	return s;
}

inline AnsiString IntZeroFill_8(int value)
{
    String s;
    s.printf("%08d",value);
    return s;
}

inline int ZMessageBox(AnsiString msg,UINT nType = MB_OK+MB_ICONWARNING)
{
	return Application->MessageBox(msg.c_str(),Application->Title.c_str(),nType);
}

inline AnsiString GetAppVersion()
{
	char* info[1024];
	bool r = GetFileVersionInfo( Application->ExeName.c_str(),0,sizeof(info),info);	
	if( !r ) return "";
	
	unsigned int len;
	void* buf;
	r = VerQueryValue( info, "\\", &buf, &len );
	if( !r ) return "";
	
	VS_FIXEDFILEINFO* p = (VS_FIXEDFILEINFO*)buf;
	AnsiString verstr;
	verstr = String(HIWORD(p->dwFileVersionMS))+"."
			+ String(LOWORD(p->dwFileVersionMS))+"."
			+ String(HIWORD(p->dwFileVersionLS))+"."
			+ String(LOWORD(p->dwFileVersionLS));
	return verstr;
}


/*
void __fastcall TFmDistMan::txtTotalMoneyKeyPress(TObject *Sender, char &Key)
{
    if( txtTotalMoney->SelText == txtTotalMoney->Text ) bPoint = false;

	if( Key == 13 || Key == 27 || Key == '-') return;
    if( Key == 8 ) { txtTotalMoney->SelectAll(); Key = 0; return; }

    if( Key == '.' )
    {
        txtTotalMoney->SelStart = txtTotalMoney->Text.Pos(".");
        bPoint = true;
        nPopos = 0 + (int)bPoint;
    }
 	if( Key < '0' || Key > '9' ) Key = 0;
    else nPopos = (nPopos+1) * (int)bPoint;
}
//---------------------------------------------------------------------------

void __fastcall TFmDistMan::txtTotalMoneyChange(TObject *Sender)
{
    if( bFormatMoney ) return;
    bFormatMoney = true;

    txtTotalMoney->Text = RemoveThousandChar( txtTotalMoney->Text );
    txtTotalMoney->Text = AddThousandChar( txtTotalMoney->Text );
    txtTotalMoney->SelStart = txtTotalMoney->Text.Pos(".")-1+nPopos;
    txtTotalMoney->SelLength = 0;

    bFormatMoney = false;
}
//---------------------------------------------------------------------------
*/

#endif // __BCBEXT_H__

#endif // __BORLANDC__