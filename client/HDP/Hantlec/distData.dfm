object distForm: TdistForm
  Left = 112
  Top = 25
  Width = 1126
  Height = 811
  Caption = #28165#20998#25968#25454
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 1118
    Height = 29
    Caption = 'ToolBar1'
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 29
    Width = 1118
    Height = 728
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 1
    object Panel2: TPanel
      Left = 957
      Top = 1
      Width = 160
      Height = 726
      Align = alRight
      TabOrder = 0
      object Label1: TLabel
        Left = 7
        Top = 40
        Width = 48
        Height = 13
        Caption = #26085'     '#26399'   '
      end
      object Label2: TLabel
        Left = 7
        Top = 82
        Width = 45
        Height = 13
        Caption = #22330'     '#27425'  '
      end
      object Label3: TLabel
        Left = 7
        Top = 124
        Width = 48
        Height = 13
        Caption = #20132' '#25442' '#21495'  '
      end
      object Label7: TLabel
        Left = 7
        Top = 161
        Width = 60
        Height = 13
        Caption = #21305#37197#29366#24577'    '
      end
      object Label4: TLabel
        Left = 6
        Top = 198
        Width = 48
        Height = 13
        Caption = #20449#23553#26631#24535
      end
      object btnRefresh: TButton
        Left = 47
        Top = 521
        Width = 73
        Height = 25
        Caption = #21047#26032
        TabOrder = 0
        OnClick = btnRefreshClick
      end
      object btnExit: TButton
        Left = 47
        Top = 577
        Width = 73
        Height = 25
        Caption = #36864#20986
        TabOrder = 1
        OnClick = btnExitClick
      end
      object txtDate: TEdit
        Left = 71
        Top = 40
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
      end
      object txtSession: TEdit
        Left = 71
        Top = 80
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 3
      end
      object txtNetno: TComboBox
        Left = 71
        Top = 120
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 4
        OnCloseUp = txtNetnoCloseUp
        OnDropDown = txtNetnoDropDown
      end
      object txtMerge: TComboBox
        Left = 71
        Top = 156
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 5
        Text = #20840'    '#37096
        OnCloseUp = txtMergeCloseUp
        Items.Strings = (
          #20840'    '#37096
          #24050#21305#37197
          #26410#21305#37197)
      end
      object cbType: TComboBox
        Left = 70
        Top = 193
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 6
        Text = #20840'    '#37096
        OnCloseUp = txtMergeCloseUp
        Items.Strings = (
          #20840'    '#37096
          #31080#25454
          #20449#23553)
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 956
      Height = 726
      Align = alClient
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 954
        Height = 724
        Align = alClient
        TabOrder = 0
        object txtDetail: TListView
          Left = 1
          Top = 1
          Width = 952
          Height = 703
          Align = alClient
          Color = clMoneyGreen
          Columns = <
            item
              Caption = #24207#21495
              Width = 70
            end
            item
              Caption = #20132#25442#21495
              Width = 80
            end
            item
              Caption = #24080#21495
              Width = 115
            end
            item
              Caption = #20973#35777#21495
              Width = 80
            end
            item
              Caption = #20973#35777#31867#22411
              Width = 80
            end
            item
              Alignment = taRightJustify
              Caption = #37329#39069
              Width = 100
            end
            item
              Caption = #21305#37197#29366#24577
              Width = 80
            end
            item
              Caption = #21305#37197#27969#27700
              Width = 80
            end
            item
              Caption = #20449#23553#26631#24535
              Width = 80
            end
            item
              Caption = #28165#20998#27169#24335
              Width = 80
            end
            item
              Caption = #28165#20998#26588#21592
              Width = 80
            end>
          ReadOnly = True
          RowSelect = True
          TabOrder = 0
          ViewStyle = vsReport
          OnColumnClick = txtDetailColumnClick
          OnDblClick = txtDetailDblClick
        end
        object stBar: TStatusBar
          Left = 1
          Top = 704
          Width = 952
          Height = 19
          Panels = <
            item
              Width = 180
            end
            item
              Width = 180
            end
            item
              Width = 180
            end
            item
              Width = 180
            end
            item
              Width = 50
            end>
          SimplePanel = False
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
        Caption = #36864' '#20986'[&E]'
      end
    end
    object N2: TMenuItem
      Caption = #25805#20316'[&O]'
      object N3: TMenuItem
        Caption = #26597#25214'[&Q]'
        OnClick = N3Click
      end
      object R1: TMenuItem
        Caption = #21047#26032'[&R]'
        OnClick = R1Click
      end
      object N6: TMenuItem
        Caption = #32593#28857#29366#24577'[&N]'
      end
    end
    object N4: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]'
      end
    end
  end
  object FindDlg: TFindDialog
    OnFind = FindDlgFind
    Left = 418
    Top = 202
  end
end
