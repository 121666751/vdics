object FmVchtype: TFmVchtype
  Left = 237
  Top = 172
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #20973#35777#31867#22411
  ClientHeight = 427
  ClientWidth = 743
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
    Width = 743
    Height = 427
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 496
      Height = 425
      Align = alLeft
      Caption = 'Panel2'
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 494
        Height = 41
        Align = alTop
        BevelOuter = bvNone
        Caption = #20973#35777#31867#22411#21015#34920
        Color = clMoneyGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object lstVchtype: TListView
        Left = 1
        Top = 42
        Width = 494
        Height = 382
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #20132#25442#21306#22495
            Width = 60
          end
          item
            Caption = #20973#35777#31867#22411'('#25171#30721')'
            Width = 90
          end
          item
            Caption = #20973#35777#31867#22411'('#27969#31243')'
            Width = 90
          end
          item
            Caption = #29256#38754#31867#22411'('#35782#21035')'
            Width = 90
          end
          item
            Caption = #20973#35777#21517#31216
            Width = 80
          end
          item
            Caption = #20511#36151#26631#24535
            Width = 60
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
        OnClick = lstVchtypeClick
      end
    end
    object Panel3: TPanel
      Left = 497
      Top = 1
      Width = 245
      Height = 425
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 14
        Top = 23
        Width = 221
        Height = 266
        Caption = #25968#25454
        TabOrder = 0
        object Label1: TLabel
          Left = 3
          Top = 69
          Width = 93
          Height = 13
          Caption = #20973#35777#31867#22411'('#25171#30721')     '
        end
        object Label2: TLabel
          Left = 3
          Top = 112
          Width = 90
          Height = 13
          Caption = #20973#35777#31867#22411'('#27969#31243')    '
        end
        object Label3: TLabel
          Left = 2
          Top = 155
          Width = 90
          Height = 13
          Caption = #29256#38754#31867#22411'('#35782#21035')    '
        end
        object Label4: TLabel
          Left = 32
          Top = 231
          Width = 57
          Height = 13
          Caption = #20511#36151#26631#24535'   '
        end
        object Label5: TLabel
          Left = 32
          Top = 195
          Width = 57
          Height = 13
          Caption = #20973#35777#21517#31216'   '
        end
        object Label6: TLabel
          Left = 3
          Top = 31
          Width = 90
          Height = 13
          Caption = '          '#20132#25442#21306#22495'    '
        end
        object txtVchtype: TEdit
          Left = 98
          Top = 65
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 0
        end
        object txtType: TEdit
          Left = 98
          Top = 106
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 2
        end
        object txtModel: TEdit
          Left = 98
          Top = 149
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 3
        end
        object cbVchtype: TComboBox
          Left = 97
          Top = 227
          Width = 113
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          ItemIndex = 0
          TabOrder = 4
          Text = #20511#26041'(D)'
          Items.Strings = (
            #20511#26041'(D)'
            #36151#26041'(C)')
        end
        object txtName: TEdit
          Left = 98
          Top = 189
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 20
          TabOrder = 5
        end
        object txtArea: TEdit
          Left = 98
          Top = 27
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 6
          TabOrder = 1
        end
      end
      object btnAdd: TButton
        Left = 107
        Top = 301
        Width = 75
        Height = 25
        Caption = #22686'  '#21152
        TabOrder = 1
        OnClick = btnAddClick
      end
      object btnExit: TButton
        Left = 108
        Top = 373
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 2
        OnClick = btnExitClick
      end
      object btnModif: TButton
        Left = 107
        Top = 337
        Width = 75
        Height = 25
        Caption = #20462' '#25913
        TabOrder = 3
        OnClick = btnModifClick
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 136
    Top = 120
    object N1: TMenuItem
      Caption = #25991#20214
      object numExit: TMenuItem
        Caption = #36864#20986
        OnClick = numExitClick
      end
    end
    object N8: TMenuItem
      Caption = #24110#21161#20449#24687
      object numAbout: TMenuItem
        Caption = #20851#20110
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
