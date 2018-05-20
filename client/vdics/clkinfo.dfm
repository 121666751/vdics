object FmCkInfo: TFmCkInfo
  Left = 385
  Top = 221
  BorderStyle = bsSingle
  Caption = #26588#21592#20449#24687
  ClientHeight = 400
  ClientWidth = 557
  Color = clMoneyGreen
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 16
    Top = 5
    Width = 265
    Height = 244
    Caption = #26588#21592#20449#24687
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 10
      Top = 34
      Width = 94
      Height = 15
      Caption = '*'#26588#21592#21495' D:  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 9
      Top = 116
      Width = 102
      Height = 15
      Caption = '*'#22995'  '#21517' N:   '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 275
      Width = 80
      Height = 16
      Caption = #29366'  '#24577'(*):'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 9
      Top = 212
      Width = 101
      Height = 15
      Caption = ' '#36523#20221#35777' I:   '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 9
      Top = 164
      Width = 94
      Height = 15
      Caption = '*'#24615'  '#21035' S:  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 10
      Top = 80
      Width = 92
      Height = 15
      Caption = '*'#20132#25442#21306#22495':  '
    end
    object txtClkno: TEdit
      Left = 104
      Top = 29
      Width = 129
      Height = 23
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 8
      TabOrder = 0
      OnExit = txtClknoExit
    end
    object txtName: TEdit
      Left = 104
      Top = 112
      Width = 129
      Height = 23
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 20
      TabOrder = 2
    end
    object btnAud: TButton
      Left = 102
      Top = 322
      Width = 75
      Height = 25
      Caption = #22797#26680
      TabOrder = 3
    end
    object txtID: TEdit
      Left = 104
      Top = 208
      Width = 153
      Height = 23
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 20
      TabOrder = 4
    end
    object txtSex: TComboBox
      Left = 104
      Top = 159
      Width = 129
      Height = 23
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 15
      ItemIndex = 0
      TabOrder = 5
      Text = #30007
      Items.Strings = (
        #30007
        #22899)
    end
    object txtArea: TEdit
      Left = 104
      Top = 75
      Width = 129
      Height = 23
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 6
      TabOrder = 1
    end
  end
  object GroupBox2: TGroupBox
    Left = 16
    Top = 288
    Width = 265
    Height = 98
    Caption = #23494#30721
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label4: TLabel
      Left = 15
      Top = 24
      Width = 108
      Height = 15
      Caption = #36755#20837#23494#30721' E:   '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 15
      Top = 67
      Width = 108
      Height = 15
      Caption = #23494#30721#30830#35748' R:   '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = #23435#20307
      Font.Style = []
      ParentFont = False
    end
    object txtPswdOne: TMaskEdit
      Left = 112
      Top = 20
      Width = 121
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 16
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 0
    end
    object txtPswdTwo: TMaskEdit
      Left = 112
      Top = 64
      Width = 121
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      MaxLength = 16
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 1
    end
  end
  object btnSave: TBitBtn
    Left = 320
    Top = 352
    Width = 97
    Height = 33
    Caption = #20445' '#23384' &S'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = btnSaveClick
  end
  object btnExit: TBitBtn
    Left = 432
    Top = 352
    Width = 89
    Height = 33
    Caption = #36864' '#20986' &X'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = btnExitClick
  end
  object GroupBox4: TGroupBox
    Left = 296
    Top = 3
    Width = 249
    Height = 336
    Caption = #26435#38480
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object Bevel1: TBevel
      Left = 8
      Top = 252
      Width = 233
      Height = 1
    end
    object txtPrivList: TCheckListBox
      Left = 8
      Top = 19
      Width = 233
      Height = 224
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = #23435#20307
      Font.Style = []
      ImeName = #20013#25991' ('#31616#20307') - '#26234#33021' ABC'
      ItemHeight = 20
      Items.Strings = (
        #26588#21592#31649#29702
        #26426#26500#31649#29702
        #20973#35777#31649#29702
        #37197#32622#31649#29702
        #25968#25454#23548#20837
        #31080#25454#28165#20998
        #25209#37327#25195#25551
        #31080#25454#22788#29702
        #20449#23553#22788#29702
        #31080#25454#19978#20256
        #31080#25454#34917#25195)
      ParentFont = False
      Style = lbOwnerDrawFixed
      TabOrder = 0
    end
  end
  object txtRadioA: TRadioButton
    Left = 320
    Top = 270
    Width = 113
    Height = 17
    Caption = #19994#21153#26588#21592
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = txtRadioAClick
  end
  object txtRadioC: TRadioButton
    Left = 424
    Top = 270
    Width = 113
    Height = 17
    Caption = #39640#32423#31649#29702#21592
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = txtRadioCClick
  end
  object txtRadioB: TRadioButton
    Left = 320
    Top = 307
    Width = 113
    Height = 17
    Caption = #20013#32423#26588#21592
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = txtRadioBClick
  end
  object txtRadioD: TRadioButton
    Left = 424
    Top = 307
    Width = 113
    Height = 17
    Caption = #33258#23450#20041#26435#38480
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = txtRadioDClick
  end
  object ckModify: TCheckBox
    Left = 20
    Top = 264
    Width = 245
    Height = 17
    Caption = #19979#27425#30331#38470#26102#24517#39035#20462#25913#23494#30721'       '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
  end
end
