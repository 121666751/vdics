object FormTrans: TFormTrans
  Left = 32
  Top = 39
  Width = 1144
  Height = 848
  Caption = #19978#20256#25968#25454
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 775
    Width = 1136
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 1136
    Height = 29
    Caption = 'ToolBar1'
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 29
    Width = 1136
    Height = 746
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 2
    object Panel2: TPanel
      Left = 963
      Top = 1
      Width = 172
      Height = 744
      Align = alRight
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 42
        Width = 60
        Height = 13
        Caption = #26085'         '#26399'   '
      end
      object Label2: TLabel
        Left = 8
        Top = 85
        Width = 57
        Height = 13
        Caption = #22330'         '#27425'  '
      end
      object Label3: TLabel
        Left = 8
        Top = 130
        Width = 54
        Height = 13
        Caption = #20132'  '#25442'  '#21495'  '
      end
      object Label5: TLabel
        Left = 8
        Top = 172
        Width = 60
        Height = 13
        Caption = #20256#36865#29366#24577'    '
      end
      object Label4: TLabel
        Left = 7
        Top = 204
        Width = 57
        Height = 13
        Caption = #20256#36865#27425#25968'   '
      end
      object btnRefresh: TButton
        Left = 56
        Top = 574
        Width = 73
        Height = 25
        Caption = #21047#26032
        TabOrder = 0
        OnClick = btnRefreshClick
      end
      object btnExit: TButton
        Left = 56
        Top = 630
        Width = 73
        Height = 25
        Caption = #36864#20986
        TabOrder = 1
        OnClick = btnExitClick
      end
      object txtDate: TEdit
        Left = 72
        Top = 37
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
      end
      object txtSession: TEdit
        Left = 72
        Top = 80
        Width = 65
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '1'
      end
      object txtExchno: TComboBox
        Left = 72
        Top = 123
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 4
        OnChange = txtExchnoChange
        OnDropDown = txtExchnoDropDown
      end
      object txtSend: TComboBox
        Left = 72
        Top = 166
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 5
        Text = #20840'  '#37096
        OnCloseUp = txtSendCloseUp
        Items.Strings = (
          #20840'  '#37096
          #24050#19978#36865
          #26410#19978#36865)
      end
      object cbTimes: TComboBox
        Left = 71
        Top = 198
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 6
        Text = #20840'  '#37096
        OnCloseUp = txtSendCloseUp
        Items.Strings = (
          #20840'  '#37096
          #19968'  '#27425
          #22810'  '#27425)
      end
      object UpDown1: TUpDown
        Left = 137
        Top = 80
        Width = 16
        Height = 21
        Associate = txtSession
        Min = 1
        Max = 3
        Position = 1
        TabOrder = 7
        Wrap = False
        OnClick = UpDown1Click
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 962
      Height = 744
      Align = alClient
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 960
        Height = 742
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object txtDetail: TListView
          Left = 0
          Top = 0
          Width = 960
          Height = 742
          Align = alClient
          BorderStyle = bsNone
          Color = clMoneyGreen
          Columns = <
            item
              Caption = #24207#21495
            end
            item
              Caption = #20132#25442#21495
              Width = 70
            end
            item
              Caption = #24080#21495
              Width = 140
            end
            item
              Caption = #20973#35777#21495
              Width = 145
            end
            item
              Caption = #20973#35777#31867#22411
              Width = 70
            end
            item
              Alignment = taRightJustify
              Caption = #37329#39069
              Width = 80
            end
            item
              Caption = #19978#20256#29366#24577
              Width = 80
            end
            item
              Caption = #24433#20687'ID'
              Width = 120
            end
            item
              Caption = #26085#26399
              Width = 70
            end
            item
              Caption = #22330#27425
            end
            item
              Caption = #38169#35823#21407#22240
              Width = 150
            end>
          ReadOnly = True
          RowSelect = True
          PopupMenu = ErrResPopMenu
          TabOrder = 0
          ViewStyle = vsReport
          OnCustomDrawItem = txtDetailCustomDrawItem
          OnDblClick = txtDetailDblClick
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
      object E2: TMenuItem
        Caption = #28151#21512#20449#23553'[&E]'
        OnClick = E2Click
      end
      object N3: TMenuItem
        Caption = #29305#33394#19994#21153'[&S]'
        OnClick = N3Click
      end
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
  object FindDlg: TFindDialog
    OnFind = FindDlgFind
    Left = 418
    Top = 202
  end
  object ErrResPopMenu: TPopupMenu
    Left = 450
    Top = 98
    object MeForceLook: TMenuItem
      Caption = #24433#20687#26367#25442'[&D] '
      OnClick = MeForceLookClick
    end
    object R1: TMenuItem
      Caption = #27969#31243#37325#21551'[&R]'
      OnClick = R1Click
    end
  end
end
