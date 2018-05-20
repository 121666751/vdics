object FmEnv: TFmEnv
  Left = 312
  Top = 38
  Width = 1024
  Height = 724
  Caption = #20449#23553#22788#29702
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1008
    Height = 666
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 1006
      Height = 41
      Align = alTop
      TabOrder = 0
    end
    object StatusBar: TStatusBar
      Left = 1
      Top = 637
      Width = 1006
      Height = 28
      Panels = <
        item
          Width = 150
        end
        item
          Width = 150
        end
        item
          Width = 50
        end>
      SimplePanel = False
    end
    object Panel4: TPanel
      Left = 1
      Top = 42
      Width = 821
      Height = 595
      Align = alClient
      BevelOuter = bvNone
      Caption = 'Panel4'
      TabOrder = 2
      object pageCtrl: TPageControl
        Left = 0
        Top = 0
        Width = 821
        Height = 595
        ActivePage = TabSheet1
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        PopupMenu = ErrResPopMenu
        TabIndex = 0
        TabOrder = 0
        OnChange = pageCtrlChange
        object TabSheet1: TTabSheet
          Caption = #25195#25551#19994#21153#20449#24687
          object lstMerge: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #24207#21495
                Width = 60
              end
              item
                Caption = #32593#28857
                Width = 70
              end
              item
                Caption = #25991#20214#21517
                Width = 140
              end
              item
                Caption = #29256#38754#31867#22411
                Width = 70
              end
              item
                Alignment = taCenter
                Caption = #20973#35777#31867#22411
                Width = 70
              end
              item
                Alignment = taRightJustify
                Caption = #37329#39069
                Width = 80
              end
              item
                Caption = #35782#21035#29366#24577
                Width = 70
              end
              item
                Caption = #21305#37197#29366#24577
                Width = 70
              end
              item
                Caption = #20027#38468#20214#26631#35782
                Width = 80
              end
              item
                Caption = #20027#20214#24207#21495
                Width = 80
              end
              item
                Caption = #20256#36865#29366#24577
                Width = 70
              end
              item
                Caption = #24433#20687'ID'
                Width = 120
              end
              item
                Caption = #38169#35823#21407#22240
                Width = 60
              end>
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            MultiSelect = True
            ReadOnly = True
            RowSelect = True
            ParentFont = False
            PopupMenu = ErrResPopMenu
            TabOrder = 0
            ViewStyle = vsReport
            OnDblClick = lstMergeDblClick
          end
        end
        object TabSheet2: TTabSheet
          Caption = #20449#23553#20449#24687
          ImageIndex = 1
          object lstEnvs: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #24207#21495
                Width = 80
              end
              item
                Caption = #32593#28857
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
                Width = 100
              end
              item
                Caption = #21305#37197#29366#24577
                Width = 80
              end
              item
                Caption = #21305#37197#27969#27700
                Width = 80
              end>
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ReadOnly = True
            RowSelect = True
            ParentFont = False
            TabOrder = 0
            ViewStyle = vsReport
            OnColumnClick = lstEnvsColumnClick
            OnDblClick = lstEnvsDblClick
          end
        end
        object TabSheet3: TTabSheet
          Caption = #20027#31080#19994#21153' '
          ImageIndex = 2
          object lstSpecial: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
            Columns = <
              item
                Caption = #24207#21495
                Width = 80
              end
              item
                Caption = #25991#20214#21517
                Width = 120
              end
              item
                Caption = #31080#25454#31867#22411
                Width = 80
              end
              item
                Alignment = taRightJustify
                Caption = #37329#39069
                Width = 100
              end
              item
                Caption = #26680#25171#29366#24577
                Width = 150
              end>
            TabOrder = 0
            ViewStyle = vsReport
          end
          object lstMain: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
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
                Width = 140
              end
              item
                Caption = #31080#25454#31867#22411
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
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 1
            ViewStyle = vsReport
            OnDblClick = lstMainDblClick
          end
        end
        object TabSheet4: TTabSheet
          Caption = #21333#31508#19994#21153#26680#25171' '
          ImageIndex = 3
          object lstSingle: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
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
                Width = 120
              end
              item
                Caption = #31080#25454#31867#22411
                Width = 80
              end
              item
                Alignment = taRightJustify
                Caption = #37329#39069
                Width = 100
              end
              item
                Caption = #26680#25171#29366#24577
                Width = 80
              end>
            ReadOnly = True
            RowSelect = True
            PopupMenu = ErrResPopMenu
            TabOrder = 0
            ViewStyle = vsReport
            OnDblClick = lstSingleDblClick
          end
        end
        object TabSheet5: TTabSheet
          Caption = #22810#31508#19994#21153#26680#25171' '
          ImageIndex = 4
          object lstMulti: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
            Color = clMoneyGreen
            Columns = <
              item
                Caption = #24207#21495
                Width = 80
              end
              item
                Caption = #32593#28857#21495
                Width = 80
              end
              item
                Caption = #25991#20214#21517
                Width = 140
              end
              item
                Caption = #31080#25454#31867#22411
                Width = 80
              end
              item
                Alignment = taRightJustify
                Caption = #37329#39069
                Width = 100
              end
              item
                Caption = #20027#38468#20214
                Width = 80
              end
              item
                Caption = #26680#25171#29366#24577
                Width = 80
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
            OnDblClick = lstMultiDblClick
          end
        end
        object TabSheet6: TTabSheet
          Caption = #20154#34892#25968#25454
          ImageIndex = 5
          object txtDetail: TListView
            Left = 0
            Top = 0
            Width = 813
            Height = 567
            Align = alClient
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
                Width = 115
              end
              item
                Caption = #20973#35777#21495
                Width = 60
              end
              item
                Caption = #20973#35777#31867#22411
                Width = 60
              end
              item
                Alignment = taRightJustify
                Caption = #37329#39069
                Width = 80
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
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
            OnDblClick = txtDetailDblClick
          end
        end
      end
    end
    object Panel3: TPanel
      Left = 822
      Top = 42
      Width = 185
      Height = 595
      Align = alRight
      TabOrder = 3
      object Label2: TLabel
        Left = 8
        Top = 40
        Width = 54
        Height = 13
        Caption = #26085'       '#26399'   '
      end
      object Label4: TLabel
        Left = 8
        Top = 82
        Width = 51
        Height = 13
        Caption = #22330'       '#27425'  '
      end
      object Label5: TLabel
        Left = 8
        Top = 124
        Width = 54
        Height = 13
        Caption = #20132'  '#25442'  '#21495'  '
      end
      object Label6: TLabel
        Left = 8
        Top = 166
        Width = 57
        Height = 13
        Caption = #35782#21035#29366#24577'   '
      end
      object Label7: TLabel
        Left = 8
        Top = 208
        Width = 60
        Height = 13
        Caption = #21305#37197#29366#24577'    '
      end
      object Label8: TLabel
        Left = 7
        Top = 243
        Width = 60
        Height = 13
        Caption = #32593#28857#29366#24577'    '
      end
      object txtDate: TEdit
        Left = 72
        Top = 40
        Width = 81
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 0
      end
      object txtSession: TEdit
        Left = 72
        Top = 80
        Width = 65
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ReadOnly = True
        TabOrder = 1
        Text = '1'
      end
      object cmbNetno: TComboBox
        Left = 72
        Top = 120
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        Sorted = True
        TabOrder = 2
        OnCloseUp = cmbNetnoCloseUp
        OnDropDown = cmbNetnoDropDown
      end
      object cmbIdentify: TComboBox
        Left = 72
        Top = 160
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 3
        Text = #20840'    '#37096
        OnCloseUp = cmbIdentifyCloseUp
        Items.Strings = (
          #20840'    '#37096
          #24050#35782#21035
          #26410#35782#21035
          #22833'    '#36133)
      end
      object cmbMerge: TComboBox
        Left = 72
        Top = 203
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 4
        Text = #20840'    '#37096
        OnCloseUp = cmbMergeCloseUp
        Items.Strings = (
          #20840'    '#37096
          #24050#21305#37197
          #26410#21305#37197)
      end
      object cmbNetState: TComboBox
        Left = 71
        Top = 238
        Width = 81
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 5
        Text = #26410#25552#20132
        OnCloseUp = cmbNetStateCloseUp
        Items.Strings = (
          #26410#25552#20132
          #24050#25552#20132
          #22788#29702#20013)
      end
      object btnMerge: TButton
        Left = 62
        Top = 498
        Width = 75
        Height = 25
        Caption = #21305#37197
        Enabled = False
        TabOrder = 6
        OnClick = btnMergeClick
      end
      object btnExit: TButton
        Left = 62
        Top = 602
        Width = 73
        Height = 25
        Caption = #36864#20986
        TabOrder = 7
        OnClick = btnExitClick
      end
      object btnSubmit: TButton
        Left = 62
        Top = 551
        Width = 75
        Height = 25
        Caption = #25552#20132
        Enabled = False
        TabOrder = 8
        OnClick = btnSubmitClick
      end
      object btnRefresh: TButton
        Left = 64
        Top = 449
        Width = 73
        Height = 25
        Caption = #21047#26032
        TabOrder = 9
        OnClick = btnRefreshClick
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
        TabOrder = 10
        Wrap = False
        OnChanging = UpDown1Changing
        OnClick = UpDown1Click
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
        Caption = #25552#20132#21305#37197'[&M]'
      end
      object R1: TMenuItem
        Caption = #21047#26032#25968#25454'[&R]'
        OnClick = R1Click
      end
      object F1: TMenuItem
        Caption = #26597#25214#25968#25454'[&F]'
        OnClick = F1Click
      end
      object btnReSubmit: TMenuItem
        Caption = #22810#27425#25552#20132'[&S]'
        Enabled = False
        OnClick = btnReSubmitClick
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
      Caption = #21333#31508#21024#38500'[&D] ...'
      OnClick = MeForceLookClick
    end
    object B1: TMenuItem
      Caption = #32593#28857#21024#38500'[&B]'
      OnClick = B1Click
    end
  end
  object FindDlg: TFindDialog
    OnFind = FindDlgFind
    Left = 208
    Top = 144
  end
end
