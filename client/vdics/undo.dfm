object FormUndo: TFormUndo
  Left = 196
  Top = 222
  Width = 782
  Height = 657
  Caption = #24453#22788#29702#19994#21153#20449#24687#26597#35810
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 774
    Height = 603
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
    object StatusBar: TStatusBar
      Left = 1
      Top = 583
      Width = 772
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
      Width = 772
      Height = 288
      Align = alTop
      Caption = 'Panel2'
      TabOrder = 1
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 384
        Height = 286
        Align = alLeft
        BevelOuter = bvLowered
        Caption = 'Panel4'
        TabOrder = 0
        object Panel8: TPanel
          Left = 1
          Top = 1
          Width = 382
          Height = 41
          Align = alTop
          Caption = #21333#31080#19994#21153
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object Panel9: TPanel
          Left = 1
          Top = 42
          Width = 382
          Height = 243
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel9'
          TabOrder = 1
          object lstVouchers: TListView
            Left = 0
            Top = 0
            Width = 382
            Height = 243
            Align = alClient
            BevelInner = bvNone
            BevelOuter = bvNone
            BorderStyle = bsNone
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #27969#27700#21495
                Width = 80
              end
              item
                Caption = #20132#25442#21495
                Width = 100
              end
              item
                Caption = #25991#20214#21517
                Width = 180
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
      end
      object Panel5: TPanel
        Left = 385
        Top = 1
        Width = 386
        Height = 286
        Align = alClient
        BevelOuter = bvLowered
        Caption = 'Panel5'
        TabOrder = 1
        object Panel10: TPanel
          Left = 1
          Top = 1
          Width = 384
          Height = 41
          Align = alTop
          Caption = #20449#23553#19994#21153
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object Panel11: TPanel
          Left = 1
          Top = 42
          Width = 384
          Height = 243
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel9'
          TabOrder = 1
          object lstEnv: TListView
            Left = 0
            Top = 0
            Width = 384
            Height = 243
            Align = alClient
            BorderStyle = bsNone
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #27969#27700#21495
                Width = 80
              end
              item
                Caption = #20132#25442#21495
                Width = 100
              end
              item
                Caption = #25991#20214#21517
                Width = 180
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 289
      Width = 772
      Height = 294
      Align = alClient
      Caption = 'Panel3'
      TabOrder = 2
      object Panel6: TPanel
        Left = 1
        Top = 1
        Width = 384
        Height = 292
        Align = alLeft
        BevelOuter = bvLowered
        Caption = 'Panel6'
        TabOrder = 0
        object Panel12: TPanel
          Left = 1
          Top = 1
          Width = 382
          Height = 41
          Align = alTop
          Caption = #29305#33394#19994#21153
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object Panel13: TPanel
          Left = 1
          Top = 42
          Width = 382
          Height = 249
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Panel9'
          TabOrder = 1
          object lstWs: TListView
            Left = 0
            Top = 0
            Width = 382
            Height = 249
            Align = alClient
            BorderStyle = bsNone
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #27969#27700#21495
                Width = 80
              end
              item
                Caption = #20132#25442#21495
                Width = 100
              end
              item
                Caption = #25991#20214#21517
                Width = 180
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
      end
      object Panel7: TPanel
        Left = 385
        Top = 1
        Width = 386
        Height = 292
        Align = alClient
        BevelOuter = bvLowered
        TabOrder = 1
        object btnRefresh: TButton
          Left = 160
          Top = 120
          Width = 75
          Height = 25
          Caption = #21047'  '#26032
          TabOrder = 0
          OnClick = btnRefreshClick
        end
        object Button2: TButton
          Left = 160
          Top = 184
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          TabOrder = 1
          OnClick = Button2Click
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
        OnClick = E1Click
      end
    end
    object N2: TMenuItem
      Caption = #25805#20316'[&O]'
      object R1: TMenuItem
        Caption = #21047#26032'[&R]'
        OnClick = R1Click
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
