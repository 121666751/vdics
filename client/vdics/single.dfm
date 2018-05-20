object FmVoucher: TFmVoucher
  Left = 230
  Top = 99
  Width = 933
  Height = 752
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #21333#31080#22788#29702
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
  object StatusBar: TStatusBar
    Left = 0
    Top = 679
    Width = 925
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 925
    Height = 29
    Caption = 'ToolBar1'
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 29
    Width = 925
    Height = 650
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object Panel2: TPanel
      Left = 752
      Top = 1
      Width = 172
      Height = 648
      Align = alRight
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 44
        Width = 48
        Height = 13
        Caption = #26085'     '#26399'   '
      end
      object Label2: TLabel
        Left = 8
        Top = 82
        Width = 45
        Height = 13
        Caption = #22330'     '#27425'  '
      end
      object Label3: TLabel
        Left = 8
        Top = 124
        Width = 48
        Height = 13
        Caption = #20132' '#25442' '#21495'  '
      end
      object Label4: TLabel
        Left = 8
        Top = 166
        Width = 57
        Height = 13
        Caption = #35782#21035#29366#24577'   '
      end
      object Label5: TLabel
        Left = 8
        Top = 208
        Width = 60
        Height = 13
        Caption = #21305#37197#29366#24577'    '
      end
      object btnRefresh: TButton
        Left = 56
        Top = 478
        Width = 73
        Height = 25
        Caption = #21047#26032
        TabOrder = 0
        OnClick = btnRefreshClick
      end
      object btnExit: TButton
        Left = 56
        Top = 528
        Width = 73
        Height = 25
        Caption = #36864#20986
        TabOrder = 1
        OnClick = btnExitClick
      end
      object txtDate: TEdit
        Left = 72
        Top = 40
        Width = 78
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
      end
      object txtSession: TEdit
        Left = 72
        Top = 80
        Width = 66
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ReadOnly = True
        TabOrder = 3
        Text = '1'
      end
      object txtNetno: TComboBox
        Left = 72
        Top = 120
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 4
        OnChange = txtNetnoChange
        OnDropDown = txtNetnoDropDown
      end
      object txtIdentify: TComboBox
        Left = 72
        Top = 160
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 5
        Text = #20840'    '#37096
        OnCloseUp = txtIdentifyCloseUp
        Items.Strings = (
          #20840'    '#37096
          #24050#35782#21035
          #26410#35782#21035
          #22833'    '#36133)
      end
      object txtMerge: TComboBox
        Left = 72
        Top = 203
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
          #24050#21305#37197
          #26410#21305#37197)
      end
      object btnMerge: TButton
        Left = 56
        Top = 432
        Width = 75
        Height = 25
        Caption = #21305#37197
        TabOrder = 7
        OnClick = btnMergeClick
      end
      object UpDown1: TUpDown
        Left = 138
        Top = 80
        Width = 16
        Height = 21
        Associate = txtSession
        Min = 1
        Max = 3
        Position = 1
        TabOrder = 8
        Wrap = False
        OnClick = UpDown1Click
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 751
      Height = 648
      Align = alClient
      AutoSize = True
      BevelOuter = bvNone
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 751
        Height = 648
        Align = alClient
        TabOrder = 0
        object txtDetail: TListView
          Left = 1
          Top = 1
          Width = 749
          Height = 646
          Align = alClient
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clMoneyGreen
          Columns = <
            item
              Caption = #24207#21495
              Width = 80
            end
            item
              Caption = #20132#25442#21495
              Width = 80
            end
            item
              Caption = #24080#21495
              Width = 120
            end
            item
              Caption = #20973#35777#21495
              Width = 145
            end
            item
              Caption = #20973#35777#31867#22411
              Width = 80
            end
            item
              Alignment = taRightJustify
              Caption = #37329#39069
              Width = 80
            end
            item
              Caption = #35782#21035#29366#24577
              Width = 80
            end
            item
              Caption = #21305#37197#29366#24577
              Width = 80
            end>
          MultiSelect = True
          ReadOnly = True
          RowSelect = True
          PopupMenu = ErrResPopMenu
          TabOrder = 0
          ViewStyle = vsReport
          OnColumnClick = txtDetailColumnClick
          OnDblClick = txtDetailDblClick
          OnSelectItem = txtDetailSelectItem
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
      object M1: TMenuItem
        Caption = #21305#37197'[&M]'
        OnClick = M1Click
      end
      object R1: TMenuItem
        Caption = #21047#26032'[&R]'
        OnClick = R1Click
      end
      object F1: TMenuItem
        Caption = #26597#25214'[&F]'
        OnClick = F1Click
      end
    end
    object H1: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]'
      end
    end
  end
  object ErrResPopMenu: TPopupMenu
    Left = 450
    Top = 98
    object MeForceLook: TMenuItem
      Caption = #21024#38500'[&D] ...'
      OnClick = MeForceLookClick
    end
  end
  object FindDlg: TFindDialog
    OnFind = FindDlgFind
    Left = 418
    Top = 202
  end
end
