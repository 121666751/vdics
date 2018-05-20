object FormSendSpec: TFormSendSpec
  Left = 104
  Top = 113
  Width = 1068
  Height = 796
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #29305#33394#19994#21153#19978#20256
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
    Width = 1060
    Height = 742
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 0
    object Panel2: TPanel
      Left = 859
      Top = 1
      Width = 200
      Height = 740
      Align = alRight
      BevelOuter = bvLowered
      TabOrder = 0
      object Label3: TLabel
        Left = 24
        Top = 88
        Width = 54
        Height = 13
        Caption = #20132'  '#25442'  '#21495'  '
      end
      object Label5: TLabel
        Left = 23
        Top = 118
        Width = 60
        Height = 13
        Caption = #20256#36865#29366#24577'    '
      end
      object Label4: TLabel
        Left = 22
        Top = 150
        Width = 57
        Height = 13
        Caption = #20256#36865#27425#25968'   '
      end
      object Label2: TLabel
        Left = 24
        Top = 55
        Width = 54
        Height = 13
        Caption = #22330'        '#27425'  '
      end
      object Label1: TLabel
        Left = 24
        Top = 24
        Width = 57
        Height = 13
        Caption = #26085'        '#26399'   '
      end
      object txtExchno: TComboBox
        Left = 88
        Top = 81
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 0
        OnChange = txtExchnoChange
        OnDropDown = txtExchnoDropDown
      end
      object btnRefresh: TButton
        Left = 72
        Top = 556
        Width = 73
        Height = 25
        Caption = #21047#26032
        TabOrder = 1
        OnClick = btnRefreshClick
      end
      object btnExit: TButton
        Left = 72
        Top = 612
        Width = 73
        Height = 25
        Caption = #36864#20986
        TabOrder = 2
        OnClick = btnExitClick
      end
      object txtSend: TComboBox
        Left = 87
        Top = 112
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 3
        Text = #20840'  '#37096
        OnCloseUp = txtSendCloseUp
        Items.Strings = (
          #20840'  '#37096
          #24050#19978#36865
          #26410#19978#36865)
      end
      object cbTimes: TComboBox
        Left = 86
        Top = 144
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 4
        Text = #20840'  '#37096
        OnCloseUp = txtSendCloseUp
        Items.Strings = (
          #20840'  '#37096
          #19968'  '#27425
          #22810'  '#27425)
      end
      object UpDown1: TUpDown
        Left = 153
        Top = 51
        Width = 16
        Height = 21
        Associate = txtSession
        Min = 1
        Max = 3
        Position = 1
        TabOrder = 5
        Wrap = False
        OnClick = UpDown1Click
      end
      object txtDate: TEdit
        Left = 88
        Top = 20
        Width = 77
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 6
      end
      object txtSession: TEdit
        Left = 88
        Top = 51
        Width = 65
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ReadOnly = True
        TabOrder = 7
        Text = '1'
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 858
      Height = 740
      Align = alClient
      BevelOuter = bvLowered
      Caption = 'Panel3'
      TabOrder = 1
      object Panel5: TPanel
        Left = 1
        Top = 1
        Width = 856
        Height = 738
        Align = alClient
        Caption = 'Panel5'
        TabOrder = 0
        object lstSend: TListView
          Left = 1
          Top = 1
          Width = 854
          Height = 717
          Align = alClient
          BorderStyle = bsNone
          Color = clMoneyGreen
          Columns = <
            item
              Caption = #24207#21495
              Width = 80
            end
            item
              Caption = #32593#28857#21495
              Width = 60
            end
            item
              Caption = #25991#20214#21517
              Width = 160
            end
            item
              Caption = #31080#25454#31867#22411
              Width = 80
            end
            item
              Alignment = taRightJustify
              Caption = #37329#39069
              Width = 90
            end
            item
              Caption = #20256#36865#29366#24577
              Width = 70
            end
            item
              Caption = #24433#20687'ID'
              Width = 90
            end
            item
              Caption = #26085#26399
              Width = 80
            end
            item
              Caption = #22330#27425
              Width = 60
            end
            item
              Caption = #38169#35823#21407#22240
              Width = 140
            end>
          ReadOnly = True
          RowSelect = True
          PopupMenu = ErrResPopMenu
          TabOrder = 0
          ViewStyle = vsReport
          OnCustomDrawItem = lstSendCustomDrawItem
          OnDblClick = lstSendDblClick
        end
        object StatusBar: TStatusBar
          Left = 1
          Top = 718
          Width = 854
          Height = 19
          Panels = <
            item
              Width = 200
            end
            item
              Width = 200
            end
            item
              Width = 200
            end>
          SimplePanel = False
        end
      end
    end
  end
  object ErrResPopMenu: TPopupMenu
    Left = 450
    Top = 98
    object MeForceLook: TMenuItem
      Caption = #24433#20687#26367#25442'[&D] '
      OnClick = MeForceLookClick
    end
    object F1: TMenuItem
      Caption = #27969#31243#37325#21551'[&F]'
      OnClick = F1Click
    end
  end
  object FindDlg: TFindDialog
    OnFind = FindDlgFind
    Left = 418
    Top = 202
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
      object N4: TMenuItem
        Caption = #26597'    '#25214'[&F]'
        OnClick = N4Click
      end
    end
    object N7: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]'
      end
    end
  end
end
