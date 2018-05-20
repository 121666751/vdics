object FmClerks: TFmClerks
  Left = 263
  Top = 246
  Width = 542
  Height = 395
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #26588#21592#21015#34920
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
    Width = 534
    Height = 361
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 392
      Height = 359
      Align = alLeft
      BevelOuter = bvLowered
      Caption = 'Panel2'
      TabOrder = 0
      object txtClerkList: TListView
        Left = 8
        Top = 8
        Width = 369
        Height = 341
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #26588#21592#21495
            Width = 80
          end
          item
            Caption = #26588#21592#21517
            Width = 80
          end
          item
            Caption = #29366#24577
          end
          item
            Caption = #31867#22411
            Width = 70
          end
          item
            Caption = #20132#25442#21306#22495
            Width = 80
          end>
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
      end
    end
    object TPanel
      Left = 393
      Top = 1
      Width = 140
      Height = 359
      Align = alClient
      BevelOuter = bvLowered
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 16
        Top = 0
        Width = 113
        Height = 249
        TabOrder = 0
        object btnAdd: TButton
          Left = 14
          Top = 40
          Width = 89
          Height = 33
          Caption = #22686'   '#21152
          TabOrder = 0
          OnClick = btnAddClick
        end
        object btnMod: TButton
          Left = 14
          Top = 104
          Width = 89
          Height = 33
          Caption = #20462'   '#25913
          TabOrder = 1
          OnClick = btnModClick
        end
        object btnDel: TButton
          Left = 14
          Top = 168
          Width = 89
          Height = 33
          Caption = #21024'   '#38500
          TabOrder = 2
          OnClick = btnDelClick
        end
      end
      object Button4: TButton
        Left = 33
        Top = 289
        Width = 89
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 1
        OnClick = Button4Click
      end
    end
  end
end
