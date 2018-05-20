object FormC2E: TFormC2E
  Left = 336
  Top = 105
  Width = 575
  Height = 301
  Caption = #20449#23553#25903#31080#21305#37197
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
    Width = 567
    Height = 186
    Align = alTop
    BevelOuter = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
    object matchPage: TPageControl
      Left = 1
      Top = 1
      Width = 565
      Height = 184
      Hint = #20449#23553
      ActivePage = ChkTab
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabIndex = 1
      TabOrder = 0
      object EnvTab: TTabSheet
        Caption = #20449#23553#26410#21305#37197
        object EnvList: TListView
          Left = 0
          Top = 0
          Width = 557
          Height = 153
          Hint = #20449#23553#26410#21305#37197#21015#34920
          Align = alClient
          Columns = <
            item
              Caption = #20973#35777#21495
              Width = 80
            end
            item
              Caption = #24080#21495
              Width = 200
            end
            item
              Caption = #37329#39069
              Width = 150
            end
            item
              Caption = #20973#35777#31181#31867
              Width = 80
            end>
          ReadOnly = True
          RowSelect = True
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
      object ChkTab: TTabSheet
        Caption = #25903#31080#26410#21305#37197
        ImageIndex = 1
        object ChkList: TListView
          Left = 0
          Top = 0
          Width = 557
          Height = 153
          Hint = #25903#31080
          Align = alClient
          Columns = <
            item
              Caption = #20973#35777#21495
              Width = 80
            end
            item
              Caption = #24080#21495
              Width = 200
            end
            item
              Caption = #37329#39069
              Width = 150
            end
            item
              Caption = #20973#35777#31181#31867
              Width = 80
            end>
          ReadOnly = True
          RowSelect = True
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
      object MergeTab: TTabSheet
        Caption = #24050#21305#37197
        ImageIndex = 2
        object MergeList: TListView
          Left = 0
          Top = 0
          Width = 557
          Height = 153
          Hint = #25903#31080
          Align = alClient
          Columns = <
            item
              Caption = #20973#35777#21495
              Width = 80
            end
            item
              Caption = #24080#21495
              Width = 200
            end
            item
              Caption = #37329#39069
              Width = 150
            end
            item
              Caption = #20973#35777#31181#31867
              Width = 80
            end>
          ReadOnly = True
          RowSelect = True
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 248
    Width = 567
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object Panel4: TPanel
    Left = 0
    Top = 186
    Width = 567
    Height = 62
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object btnOk: TBitBtn
      Left = 386
      Top = 18
      Width = 129
      Height = 29
      Caption = #30830' '#23450
      ModalResult = 1
      TabOrder = 0
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
      Spacing = 16
    end
  end
end
