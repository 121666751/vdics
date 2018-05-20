object FmVips: TFmVips
  Left = 213
  Top = 142
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #37325#28857#23458#25143#35774#32622
  ClientHeight = 373
  ClientWidth = 683
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
    Width = 683
    Height = 373
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 440
      Height = 371
      Align = alLeft
      Caption = 'Panel2'
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 438
        Height = 41
        Align = alTop
        BevelOuter = bvNone
        Caption = #37325#35201#23458#25143#35774#32622
        Color = clMoneyGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object lstVips: TListView
        Left = 1
        Top = 42
        Width = 438
        Height = 328
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #27969#27700#21495
            Width = 60
          end
          item
            Caption = #36134#21495
            Width = 120
          end
          item
            Caption = #23458#25143#21517#31216
            Width = 120
          end
          item
            Caption = #20132#25442#21495
            Width = 120
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        RowSelect = True
        ParentFont = False
        PopupMenu = ErrResPopMenu
        TabOrder = 1
        ViewStyle = vsReport
      end
    end
    object Panel3: TPanel
      Left = 441
      Top = 1
      Width = 241
      Height = 371
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 8
        Top = 55
        Width = 225
        Height = 146
        Caption = #25968#25454
        TabOrder = 0
        object Label1: TLabel
          Left = 6
          Top = 32
          Width = 60
          Height = 13
          Caption = #36134'        '#21495'    '
        end
        object Label2: TLabel
          Left = 7
          Top = 108
          Width = 48
          Height = 13
          Caption = #20132'  '#25442'  '#21495
        end
        object Label6: TLabel
          Left = 7
          Top = 68
          Width = 60
          Height = 13
          Caption = #36134#21495#21517#31216'    '
        end
        object txtExchno: TEdit
          Left = 74
          Top = 105
          Width = 143
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 12
          TabOrder = 3
        end
        object txtName: TEdit
          Left = 98
          Top = 189
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 20
          TabOrder = 0
        end
        object txtAccno: TEdit
          Left = 73
          Top = 28
          Width = 143
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 32
          TabOrder = 1
        end
        object txtAccname: TEdit
          Left = 74
          Top = 65
          Width = 143
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 60
          TabOrder = 2
        end
      end
      object btnAdd: TButton
        Left = 91
        Top = 225
        Width = 75
        Height = 25
        Caption = #22686'  '#21152
        TabOrder = 1
        OnClick = btnAddClick
      end
      object btnExit: TButton
        Left = 92
        Top = 269
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 2
        OnClick = btnExitClick
      end
    end
  end
  object ErrResPopMenu: TPopupMenu
    Left = 114
    Top = 162
    object MeForceLook: TMenuItem
      Caption = #21024#38500'[&D] ...'
      OnClick = MeForceLookClick
    end
  end
end
