object FormStatic: TFormStatic
  Left = 203
  Top = 152
  Width = 777
  Height = 409
  Caption = #19994#21153#32479#35745
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 761
    Height = 351
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
    object StatusBar: TStatusBar
      Left = 1
      Top = 331
      Width = 759
      Height = 19
      Panels = <
        item
          Width = 300
        end
        item
          Width = 300
        end
        item
          Width = 200
        end>
      SimplePanel = False
    end
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 759
      Height = 330
      Align = alClient
      Caption = 'Panel2'
      TabOrder = 1
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 384
        Height = 328
        Align = alLeft
        BevelOuter = bvLowered
        Caption = 'Panel4'
        TabOrder = 0
        object Panel9: TPanel
          Left = 1
          Top = 1
          Width = 382
          Height = 326
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel9'
          TabOrder = 0
          object DBChart1: TDBChart
            Left = 0
            Top = 0
            Width = 382
            Height = 326
            BackWall.Brush.Color = clWhite
            BackWall.Brush.Style = bsClear
            Title.Text.Strings = (
              'TDBChart')
            Align = alClient
            Color = clMoneyGreen
            TabOrder = 0
          end
        end
      end
      object Panel5: TPanel
        Left = 385
        Top = 1
        Width = 373
        Height = 328
        Align = alClient
        BevelOuter = bvLowered
        Caption = 'Panel5'
        TabOrder = 1
        object Panel11: TPanel
          Left = 1
          Top = 1
          Width = 371
          Height = 326
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel9'
          TabOrder = 0
          object DBChart2: TDBChart
            Left = 0
            Top = 0
            Width = 371
            Height = 326
            BackWall.Brush.Color = clWhite
            BackWall.Brush.Style = bsClear
            Title.Text.Strings = (
              'TDBChart')
            Align = alClient
            Color = clMoneyGreen
            TabOrder = 0
          end
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 232
    Top = 88
    object N1: TMenuItem
      Caption = #25991#20214'[&F]'
      object E1: TMenuItem
        Caption = #36864#20986'[&E]'
      end
    end
    object N2: TMenuItem
      Caption = #25805#20316'[&O]'
      object R1: TMenuItem
        Caption = #21047#26032'[&R]'
      end
    end
    object H1: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]'
      end
    end
  end
end
