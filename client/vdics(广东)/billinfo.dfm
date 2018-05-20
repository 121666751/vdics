object FormBill: TFormBill
  Left = 197
  Top = 206
  Width = 402
  Height = 352
  Caption = #31080#25454#20449#24687
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 394
    Height = 318
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 15
      Top = 24
      Width = 361
      Height = 225
      Caption = #20132#25442#20449#24687
      TabOrder = 0
      object Label1: TLabel
        Left = 22
        Top = 35
        Width = 33
        Height = 13
        Caption = #26085#26399'   '
      end
      object Label2: TLabel
        Left = 200
        Top = 35
        Width = 30
        Height = 13
        Caption = #22330#27425'  '
      end
      object Label3: TLabel
        Left = 13
        Top = 71
        Width = 39
        Height = 13
        Caption = '  '#24080#21495'   '
      end
      object Label4: TLabel
        Left = 7
        Top = 112
        Width = 45
        Height = 13
        Caption = #20973#35777#21495'   '
      end
      object Label5: TLabel
        Left = 176
        Top = 112
        Width = 66
        Height = 13
        Caption = #20973#35777#31867#22411'      '
      end
      object Label6: TLabel
        Left = 10
        Top = 152
        Width = 45
        Height = 13
        Caption = #20132#25442#21495'   '
      end
      object Label7: TLabel
        Left = 11
        Top = 184
        Width = 42
        Height = 13
        Caption = #37329'   '#39069'   '
      end
      object Label8: TLabel
        Left = 176
        Top = 148
        Width = 60
        Height = 13
        Caption = #31080#25454#31867#22411'    '
      end
      object txtSession: TEdit
        Left = 248
        Top = 31
        Width = 54
        Height = 24
        Color = clMenu
        Font.Charset = GB2312_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = #23435#20307
        Font.Style = []
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ParentFont = False
        TabOrder = 2
        Text = '1'
      end
      object UpDown: TUpDown
        Left = 302
        Top = 31
        Width = 15
        Height = 24
        Associate = txtSession
        Min = 1
        Max = 3
        Position = 1
        TabOrder = 0
        Wrap = False
      end
      object txtAccno: TEdit
        Left = 62
        Top = 67
        Width = 250
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        MaxLength = 32
        TabOrder = 3
      end
      object txtDate: TDateTimePicker
        Left = 62
        Top = 28
        Width = 105
        Height = 21
        Hint = #26085#26399
        CalAlignment = dtaLeft
        Date = 37764.4105923843
        Time = 37764.4105923843
        Color = clBtnFace
        DateFormat = dfShort
        DateMode = dmComboBox
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #23435#20307
        Font.Style = [fsBold]
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
        Kind = dtkDate
        ParseInput = False
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
      end
      object txtVchno: TEdit
        Left = 62
        Top = 108
        Width = 101
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        MaxLength = 16
        TabOrder = 4
      end
      object txtVchtype: TEdit
        Left = 238
        Top = 109
        Width = 75
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        MaxLength = 4
        TabOrder = 5
      end
      object txtExchno: TEdit
        Left = 62
        Top = 146
        Width = 101
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 6
      end
      object txtAmount: TEdit
        Left = 62
        Top = 182
        Width = 99
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 7
        OnChange = txtAmountChange
        OnEnter = txtAmountEnter
        OnKeyPress = txtAmountKeyPress
      end
      object cbMsgType: TComboBox
        Left = 240
        Top = 144
        Width = 75
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 8
        Text = #21333#29420#25903#31080
        Items.Strings = (
          #21333#29420#25903#31080
          #20854'    '#20182)
      end
      object chkMode: TCheckBox
        Left = 175
        Top = 182
        Width = 97
        Height = 17
        Caption = #26032#29256#38754
        TabOrder = 9
      end
    end
    object btnExit: TButton
      Left = 285
      Top = 272
      Width = 75
      Height = 25
      Caption = #21462#28040
      ModalResult = 2
      TabOrder = 1
    end
    object btnOk: TButton
      Left = 181
      Top = 272
      Width = 75
      Height = 25
      Caption = #30830#23450
      ModalResult = 1
      TabOrder = 2
      OnClick = btnOkClick
    end
  end
end
