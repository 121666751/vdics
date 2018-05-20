object FormSignBJ: TFormSignBJ
  Left = 380
  Top = 330
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #25552#22238#36864#31080#30331#35760
  ClientHeight = 144
  ClientWidth = 319
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 319
    Height = 144
    Align = alClient
    TabOrder = 0
    object Label1: TLabel
      Left = 18
      Top = 24
      Width = 60
      Height = 13
      Caption = #20132#25442#21495'        '
    end
    object Label2: TLabel
      Left = 16
      Top = 63
      Width = 108
      Height = 13
      Caption = #20511#26041#36864#31080#24635#31508#25968'        '
    end
    object txtExchno: TComboBox
      Left = 72
      Top = 18
      Width = 129
      Height = 22
      Style = csOwnerDrawFixed
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 16
      TabOrder = 0
      OnCloseUp = txtExchnoCloseUp
    end
    object txtDCount: TEdit
      Left = 133
      Top = 56
      Width = 66
      Height = 21
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 1
    end
    object btnSubmit: TButton
      Left = 23
      Top = 98
      Width = 75
      Height = 25
      Caption = #25552'         '#20132
      TabOrder = 2
      OnClick = btnSubmitClick
    end
    object btnExit: TButton
      Left = 231
      Top = 99
      Width = 75
      Height = 25
      Caption = #36864'          '#20986
      ModalResult = 1
      TabOrder = 3
    end
    object chkSign: TCheckBox
      Left = 216
      Top = 16
      Width = 97
      Height = 17
      Caption = #26410#30331#35760
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = chkSignClick
    end
    object btnAll: TButton
      Left = 132
      Top = 99
      Width = 75
      Height = 25
      Caption = #25209#37327#25552#20132
      TabOrder = 5
      OnClick = btnAllClick
    end
  end
end
