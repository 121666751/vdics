object VchMode: TVchMode
  Left = 299
  Top = 99
  Width = 484
  Height = 462
  Caption = #20973#35777#27169#22411
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 468
    Height = 424
    Align = alClient
    TabOrder = 0
    object Bevel2: TBevel
      Left = 24
      Top = 16
      Width = 433
      Height = 353
      Shape = bsFrame
    end
    object Label1: TLabel
      Left = 128
      Top = 40
      Width = 78
      Height = 13
      Caption = #20973#35777#21517#31216'          '
    end
    object vchname: TLabel
      Left = 224
      Top = 40
      Width = 138
      Height = 13
      Caption = '                                              '
    end
    object GroupBox2: TGroupBox
      Left = 64
      Top = 72
      Width = 129
      Height = 73
      Caption = #25195#25551#26684#24335
      TabOrder = 0
      object RadioBmp: TRadioButton
        Left = 8
        Top = 16
        Width = 97
        Height = 17
        Caption = 'BMP '#26684#24335
        TabOrder = 0
      end
      object RadioJpg: TRadioButton
        Left = 8
        Top = 48
        Width = 89
        Height = 17
        Caption = 'JPG '#26684#24335
        TabOrder = 1
      end
    end
    object GroupBox3: TGroupBox
      Left = 240
      Top = 72
      Width = 177
      Height = 89
      Caption = #25195#25551#38754
      TabOrder = 1
      object chkSideA: TCheckBox
        Left = 8
        Top = 16
        Width = 57
        Height = 17
        BiDiMode = bdRightToLeft
        Caption = #27491#38754
        Checked = True
        ParentBiDiMode = False
        State = cbChecked
        TabOrder = 0
      end
      object chkSideB: TCheckBox
        Left = 8
        Top = 56
        Width = 57
        Height = 17
        Caption = #21453#38754
        TabOrder = 1
      end
      object chkProcA: TCheckBox
        Left = 88
        Top = 16
        Width = 81
        Height = 17
        Caption = #21442#21152#39564#21360
        Checked = True
        State = cbChecked
        TabOrder = 2
        Visible = False
      end
      object chkProcB: TCheckBox
        Left = 88
        Top = 56
        Width = 73
        Height = 17
        Caption = #21442#21152#39564#21360
        TabOrder = 3
        Visible = False
      end
    end
    object GroupBox4: TGroupBox
      Left = 64
      Top = 152
      Width = 129
      Height = 73
      Caption = #22270#20687#26059#36716
      TabOrder = 2
      object Radio0: TRadioButton
        Left = 8
        Top = 16
        Width = 49
        Height = 17
        Caption = '0  '#24230
        TabOrder = 0
      end
      object Radio90: TRadioButton
        Left = 72
        Top = 16
        Width = 49
        Height = 17
        Caption = '90 '#24230
        TabOrder = 1
      end
      object Radio180: TRadioButton
        Left = 8
        Top = 48
        Width = 49
        Height = 17
        Caption = '180'#24230
        TabOrder = 2
      end
      object Radio270: TRadioButton
        Left = 72
        Top = 48
        Width = 49
        Height = 17
        Caption = '270'#24230
        TabOrder = 3
      end
    end
    object GroupBox5: TGroupBox
      Left = 64
      Top = 240
      Width = 129
      Height = 97
      Caption = #22270#20687#33394#24425
      TabOrder = 3
      object RadioRgb: TRadioButton
        Left = 8
        Top = 16
        Width = 89
        Height = 17
        Caption = 'RGB 24BIT'
        TabOrder = 0
      end
      object RadioGray: TRadioButton
        Left = 8
        Top = 40
        Width = 81
        Height = 17
        Caption = 'Gray 256'
        TabOrder = 1
      end
      object RadioBw: TRadioButton
        Left = 8
        Top = 64
        Width = 97
        Height = 17
        Caption = 'BlackWhite'
        TabOrder = 2
      end
    end
    object GroupBox1: TGroupBox
      Left = 240
      Top = 176
      Width = 177
      Height = 161
      Caption = #22270'    '#20687
      TabOrder = 4
      object Label7: TLabel
        Left = 12
        Top = 20
        Width = 57
        Height = 13
        Caption = #25195#25551#31934#24230'   '
      end
      object Label8: TLabel
        Left = 12
        Top = 52
        Width = 55
        Height = 13
        Caption = #22270#20687#23610#23544'X'
      end
      object Label9: TLabel
        Left = 12
        Top = 84
        Width = 55
        Height = 13
        Caption = #25195#25551#23610#23544'Y'
      end
      object Label10: TLabel
        Left = 72
        Top = 84
        Width = 30
        Height = 13
        Caption = '('#33521#23544')'
      end
      object Label11: TLabel
        Left = 71
        Top = 53
        Width = 30
        Height = 13
        Caption = '('#33521#23544')'
      end
      object Label12: TLabel
        Left = 80
        Top = 21
        Width = 24
        Height = 13
        Caption = '(DPI)'
      end
      object Label2: TLabel
        Left = 16
        Top = 120
        Width = 60
        Height = 13
        Caption = #25195#25551#27169#24335'    '
      end
      object txtDpi: TEdit
        Left = 112
        Top = 16
        Width = 41
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 0
      end
      object txtVchX: TEdit
        Left = 112
        Top = 48
        Width = 41
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 1
      end
      object txtVchY: TEdit
        Left = 112
        Top = 80
        Width = 41
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
      end
      object cbScanType: TComboBox
        Left = 80
        Top = 116
        Width = 73
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 3
        Text = #21333#31080#25195#25551
        Items.Strings = (
          #21333#31080#25195#25551
          #28151#21512#25195#25551
          #22996#25910#25195#25551
          #38750#31246#25195#25551)
      end
    end
    object Conform: TButton
      Left = 232
      Top = 384
      Width = 75
      Height = 25
      Caption = #30830#23450
      ModalResult = 1
      TabOrder = 5
      OnClick = ConformClick
    end
    object Cancel: TButton
      Left = 344
      Top = 384
      Width = 75
      Height = 25
      Caption = #21462#28040
      ModalResult = 2
      TabOrder = 6
      OnClick = CancelClick
    end
  end
end
