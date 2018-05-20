object FormImages: TFormImages
  Left = 61
  Top = 108
  Width = 824
  Height = 414
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #26174#31034
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
    Width = 816
    Height = 380
    Align = alClient
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 216
      Height = 378
      Align = alLeft
      Caption = 'Panel2'
      TabOrder = 0
      object lstFile: TListView
        Left = 1
        Top = 1
        Width = 214
        Height = 376
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #24207#21495
            Width = 70
          end
          item
            Caption = #25991#20214#21517
            Width = 140
          end>
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnSelectItem = lstFileSelectItem
      end
    end
    object Panel3: TPanel
      Left = 217
      Top = 1
      Width = 598
      Height = 378
      Align = alClient
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 596
        Height = 280
        Align = alTop
        TabOrder = 0
        object image: TImage
          Left = 1
          Top = 1
          Width = 594
          Height = 278
          Align = alClient
          OnMouseUp = imageMouseUp
        end
      end
      object btnExit: TButton
        Left = 456
        Top = 328
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 1
        OnClick = btnExitClick
      end
    end
  end
end
