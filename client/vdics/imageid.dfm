object FormImageID: TFormImageID
  Left = 486
  Top = 329
  Width = 441
  Height = 134
  Caption = #24433#20687'ID'#24405#20837'  '
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
    Width = 433
    Height = 100
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 24
      Top = 16
      Width = 385
      Height = 65
      Caption = #20449#24687#24405#20837
      TabOrder = 0
      object Label1: TLabel
        Left = 12
        Top = 31
        Width = 44
        Height = 13
        Caption = #24433#20687'ID   '
        Color = clBtnFace
        ParentColor = False
      end
      object txtImageID: TEdit
        Left = 60
        Top = 26
        Width = 121
        Height = 21
        MaxLength = 50
        TabOrder = 0
      end
      object btnOK: TButton
        Left = 199
        Top = 23
        Width = 75
        Height = 25
        Caption = #30830#23450
        ModalResult = 1
        TabOrder = 1
        OnClick = btnOKClick
      end
      object btnExit: TButton
        Left = 290
        Top = 23
        Width = 75
        Height = 25
        Caption = #21462#28040
        ModalResult = 2
        TabOrder = 2
      end
    end
  end
end
