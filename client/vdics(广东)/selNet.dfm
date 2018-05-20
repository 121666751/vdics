object FmSelNet: TFmSelNet
  Left = 355
  Top = 97
  Width = 543
  Height = 247
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #25195#25551#32593#28857#21495#36873#25321
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clNavy
  Font.Height = -16
  Font.Name = #23435#20307
  Font.Style = [fsBold]
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Bevel1: TBevel
    Left = 12
    Top = 12
    Width = 517
    Height = 133
    Shape = bsFrame
    Style = bsRaised
  end
  object Label2: TLabel
    Left = 18
    Top = 24
    Width = 121
    Height = 24
    AutoSize = False
    Caption = #25195#25551#20132#25442#21495#30721
    Layout = tlCenter
  end
  object Label1: TLabel
    Left = 18
    Top = 110
    Width = 111
    Height = 16
    Caption = #25195#25551#20973#35777#27169#24335' '
  end
  object Label3: TLabel
    Left = 20
    Top = 67
    Width = 102
    Height = 16
    Caption = #25195#25551#20132#25442#21306#22495
  end
  object LabelType: TLabel
    Left = 265
    Top = 29
    Width = 129
    Height = 16
    Caption = #25195#25551#20973#35777#31867#22411'   '
    Visible = False
  end
  object btnOk: TBitBtn
    Left = 237
    Top = 163
    Width = 100
    Height = 30
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 7
    TabStop = False
    OnClick = btnOkClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
    Spacing = 8
  end
  object BitBtn2: TBitBtn
    Left = 373
    Top = 163
    Width = 100
    Height = 30
    Caption = #21462#28040
    TabOrder = 3
    TabStop = False
    Kind = bkCancel
    Spacing = 8
  end
  object txtPkgNo: TComboBox
    Left = 131
    Top = 24
    Width = 118
    Height = 24
    Hint = #20174#21015#34920#20013#36873#25321#26412#27425#28165#20998#30340#32593#28857
    CharCase = ecUpperCase
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 16
    TabOrder = 0
    OnChange = txtPkgNoChange
    OnDropDown = txtPkgNoDropDown
    OnExit = txtPkgNoExit
    Items.Strings = (
      '')
  end
  object txtVchType: TComboBox
    Left = 132
    Top = 105
    Width = 115
    Height = 24
    Hint = #20174#21015#34920#20013#36873#25321#26412#27425#28165#20998#30340#32593#28857
    Style = csDropDownList
    CharCase = ecUpperCase
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 16
    TabOrder = 2
  end
  object chbSingle: TCheckBox
    Left = 263
    Top = 108
    Width = 79
    Height = 18
    Caption = #21333#20027#20214
    Checked = True
    State = cbChecked
    TabOrder = 4
  end
  object txtArea: TComboBox
    Left = 132
    Top = 62
    Width = 117
    Height = 24
    Hint = #20174#21015#34920#20013#36873#25321#26412#27425#28165#20998#30340#32593#28857
    CharCase = ecUpperCase
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 16
    TabOrder = 1
    Items.Strings = (
      '')
  end
  object cbVchtype: TCheckBox
    Left = 337
    Top = 108
    Width = 97
    Height = 17
    Caption = #25351#23450#20973#35777
    TabOrder = 5
    OnClick = cbVchtypeClick
  end
  object cbType: TComboBox
    Left = 379
    Top = 23
    Width = 134
    Height = 24
    Hint = #20174#21015#34920#20013#36873#25321#26412#27425#28165#20998#30340#32593#28857
    Style = csDropDownList
    CharCase = ecUpperCase
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 16
    TabOrder = 6
    Visible = False
  end
  object cbNewMode: TCheckBox
    Left = 437
    Top = 107
    Width = 77
    Height = 17
    Caption = #26032#29256#38754
    TabOrder = 8
    Visible = False
  end
end
