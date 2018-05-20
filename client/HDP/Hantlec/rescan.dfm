object RescanForm: TRescanForm
  Left = 192
  Top = 107
  Width = 354
  Height = 177
  Caption = #34917#25195#32593#28857#36873#25321
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 16
    Top = 16
    Width = 313
    Height = 81
    Style = bsRaised
  end
  object Bevel2: TBevel
    Left = 32
    Top = 32
    Width = 273
    Height = 49
  end
  object Label1: TLabel
    Left = 50
    Top = 50
    Width = 105
    Height = 16
    Caption = #34917#25195#32593#28857'             '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object cbNetNo: TComboBox
    Left = 130
    Top = 47
    Width = 153
    Height = 21
    Hint = #20174#21015#34920#20013#36873#25321#26412#27425#28165#20998#30340#32593#28857
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      '100000')
  end
  object btnOk: TBitBtn
    Left = 105
    Top = 108
    Width = 97
    Height = 25
    Caption = #30830#23450
    TabOrder = 1
    Kind = bkOK
    Spacing = 8
  end
  object BitBtn2: TBitBtn
    Left = 225
    Top = 109
    Width = 97
    Height = 25
    Caption = #21462#28040
    TabOrder = 2
    Kind = bkCancel
    Spacing = 8
  end
end
