//---------------------------------------------------------------------------

#include <vcl.h>
#include <zcommon.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("main.cpp", FmIC);
USEFORM("single.cpp", FmVoucher);
USEFORM("clerks.cpp", FmClerks);
USEFORM("clkinfo.cpp", FmCkInfo);
USEFORM("warrant.cpp", FormWarrant);
USEFORM("selNet.cpp", FmSelNet);
USEFORM("setupdlg.cpp", FmSetup);
USEFORM("vchmodel.cpp", VchMode);
USEFORM("vchname.cpp", Newvch);
USEFORM("waitdlg.cpp", FmWait);
USEFORM("image.cpp", FormImage);
USEFORM("more.cpp", FmEnv);
USEFORM("wsimages.cpp", FormWsImage);
USEFORM("images.cpp", FormImages);
USEFORM("wscheck.cpp", FormWsCheck);
USEFORM("Password.cpp", FormPWD);
USEFORM("bocs.cpp", FormNets);
USEFORM("vchtype.cpp", FmVchtype);
USEFORM("upload.cpp", FormTrans);
USEFORM("sendSpecForm.cpp", FormSendSpec);
USEFORM("pbcdata.cpp", FormPBC);
USEFORM("pbcimg.cpp", FormDet);
USEFORM("upLoadsumbj.cpp", FormSignBJ);
USEFORM("bjpbc.cpp", BjFormPBC);
USEFORM("datesess.cpp", FormDate);
USEFORM("newcollection.cpp", FormNWS);
USEFORM("check.cpp", FormCheck);
USEFORM("mainimage.cpp", FormMainImage);
USEFORM("sendForm.cpp", FormSend);
USEFORM("distimage.cpp", distFormImage);
USEFORM("replacescan.cpp", FormReplace);
USEFORM("envimage.cpp", FormEnvImage);
USEFORM("login.cpp", FormLogin);
USEFORM("state.cpp", FormST);
USEFORM("undo.cpp", FormUndo);
USEFORM("statics.cpp", FormStatic);
USEFORM("upLoadsum.cpp", FormSign);
USEFORM("distData.cpp", distForm);
USEFORM("imageid.cpp", FormImageID);
USEFORM("billinfo.cpp", FormBill);
USEFORM("batscan.cpp", FormScan);
USEFORM("configs.cpp", FormConfig);
//---------------------------------------------------------------------------
extern String m_XmlMsg_In;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmd, int nCmd)
{
	debugout("lpCmd = %s,nCmd = %d",lpCmd,nCmd);
	if (nCmd >0 && strlen(lpCmd)>0)
	{
		m_XmlMsg_In.sprintf("%s",lpCmd);
		debugout("xml = %s",m_XmlMsg_In.c_str());
	}
	
    try
    {  
        Application->Initialize();
        Application->CreateForm(__classid(TFmIC), &FmIC);
                 Application->CreateForm(__classid(TFormLogin), &FormLogin);
                 Application->CreateForm(__classid(TFormWarrant), &FormWarrant);
                 Application->CreateForm(__classid(TFmVoucher), &FmVoucher);
                 Application->CreateForm(__classid(TFmClerks), &FmClerks);
                 Application->CreateForm(__classid(TFmCkInfo), &FmCkInfo);
                 Application->CreateForm(__classid(TFormWarrant), &FormWarrant);
                 Application->CreateForm(__classid(TFmSelNet), &FmSelNet);
                 Application->CreateForm(__classid(TFmSetup), &FmSetup);
                 Application->CreateForm(__classid(TVchMode), &VchMode);
                 Application->CreateForm(__classid(TNewvch), &Newvch);
                 Application->CreateForm(__classid(TFmWait), &FmWait);
                 Application->CreateForm(__classid(TFormImage), &FormImage);
                 Application->CreateForm(__classid(TFmEnv), &FmEnv);
                 Application->CreateForm(__classid(TFormWsImage), &FormWsImage);
                 Application->CreateForm(__classid(TFormImages), &FormImages);
                 Application->CreateForm(__classid(TFormWsCheck), &FormWsCheck);
                 Application->CreateForm(__classid(TFormPWD), &FormPWD);
                 Application->CreateForm(__classid(TFormNets), &FormNets);
                 Application->CreateForm(__classid(TFmVchtype), &FmVchtype);
                 Application->CreateForm(__classid(TFormTrans), &FormTrans);
                 Application->CreateForm(__classid(TFormSendSpec), &FormSendSpec);
                 Application->CreateForm(__classid(TFormPBC), &FormPBC);
                 Application->CreateForm(__classid(TFormDet), &FormDet);
                 Application->CreateForm(__classid(TFormSignBJ), &FormSignBJ);
                 Application->CreateForm(__classid(TBjFormPBC), &BjFormPBC);
                 Application->CreateForm(__classid(TBjFormPBC), &BjFormPBC);
                 Application->CreateForm(__classid(TFormDate), &FormDate);
                 Application->CreateForm(__classid(TFormCheck), &FormCheck);
                 Application->CreateForm(__classid(TFormMainImage), &FormMainImage);
                 Application->CreateForm(__classid(TFormSend), &FormSend);
                 Application->CreateForm(__classid(TdistFormImage), &distFormImage);
                 Application->CreateForm(__classid(TFormReplace), &FormReplace);
                 Application->CreateForm(__classid(TFormEnvImage), &FormEnvImage);
                 Application->CreateForm(__classid(TFormST), &FormST);
                 Application->CreateForm(__classid(TFormUndo), &FormUndo);
                 Application->CreateForm(__classid(TFormStatic), &FormStatic);
                 Application->CreateForm(__classid(TFormSign), &FormSign);
                 Application->CreateForm(__classid(TdistForm), &distForm);
                 Application->CreateForm(__classid(TFormImageID), &FormImageID);
                 Application->CreateForm(__classid(TFormBill), &FormBill);
                 Application->CreateForm(__classid(TFormScan), &FormScan);
                 Application->CreateForm(__classid(TFormConfig), &FormConfig);
                 Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
