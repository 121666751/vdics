object FormST: TFormST
  Left = 264
  Top = 151
  Width = 1053
  Height = 739
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #32593#28857#22788#29702#29366#24577
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenuST
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1037
    Height = 681
    Align = alClient
    TabOrder = 0
    object Panel1: TPanel
      Left = 851
      Top = 1
      Width = 185
      Height = 679
      Align = alRight
      BevelOuter = bvLowered
      TabOrder = 0
      object Label1: TLabel
        Left = 9
        Top = 81
        Width = 54
        Height = 13
        Caption = #38454'      '#27573'    '
      end
      object Label2: TLabel
        Left = 8
        Top = 116
        Width = 48
        Height = 13
        Caption = #29366'      '#24577'  '
      end
      object Label3: TLabel
        Left = 8
        Top = 17
        Width = 51
        Height = 13
        Caption = #26085'      '#26399'   '
      end
      object Label4: TLabel
        Left = 8
        Top = 47
        Width = 48
        Height = 13
        Caption = #22330'      '#27425'  '
      end
      object cmbPhase: TComboBox
        Left = 64
        Top = 77
        Width = 89
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 0
        Text = #20840#37096
        OnChange = cmbPhaseChange
        Items.Strings = (
          #20840#37096
          #19968#27425#28165#20998
          #21333#31080#25195#25551
          #20449#23553#25195#25551
          #22996#25910#25195#25551
          #20449#23553#25552#20132)
      end
      object btnRefresh: TButton
        Left = 64
        Top = 384
        Width = 75
        Height = 25
        Caption = #21047'  '#26032
        TabOrder = 1
        OnClick = btnRefreshClick
      end
      object Button2: TButton
        Left = 64
        Top = 456
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 2
        OnClick = Button2Click
      end
      object cmbState: TComboBox
        Left = 66
        Top = 111
        Width = 86
        Height = 22
        Style = csOwnerDrawFixed
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 16
        ItemIndex = 0
        TabOrder = 3
        Text = #20840#37096
        OnChange = cmbPhaseChange
        Items.Strings = (
          #20840#37096
          #24050#36827#34892
          #26410#36827#34892)
      end
      object txtDate: TEdit
        Left = 63
        Top = 14
        Width = 84
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 4
      end
      object txtSession: TEdit
        Left = 64
        Top = 45
        Width = 72
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ReadOnly = True
        TabOrder = 5
        Text = '1'
      end
      object UpDown1: TUpDown
        Left = 136
        Top = 45
        Width = 15
        Height = 21
        Associate = txtSession
        Min = 1
        Max = 3
        Position = 1
        TabOrder = 6
        Wrap = False
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 850
      Height = 679
      Align = alClient
      BevelOuter = bvLowered
      Caption = 'Panel3'
      TabOrder = 1
      object txtDetail: TListView
        Left = 1
        Top = 1
        Width = 848
        Height = 649
        Align = alClient
        BorderStyle = bsNone
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #20132#25442#21495
            Width = 100
          end
          item
            Alignment = taRightJustify
            Caption = #20511#26041#24635#37329#39069
            Width = 120
          end
          item
            Alignment = taRightJustify
            Caption = #36151#26041#24635#37329#39069
            Width = 120
          end
          item
            Caption = #28165#20998#29366#24577
            Width = 100
          end
          item
            Caption = #21333#31080#25195#25551
            Width = 100
          end
          item
            Caption = #20449#23553#25195#25551
            Width = 100
          end
          item
            Caption = #22996#25910#25195#25551
            Width = 100
          end
          item
            Caption = #20449#23553#25552#20132
            Width = 100
          end>
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
      end
      object StatusBar: TStatusBar
        Left = 1
        Top = 650
        Width = 848
        Height = 28
        Panels = <
          item
            Text = #32593#28857#24635#25968
            Width = 100
          end
          item
            Text = #24050#28165#20998#24635#32593#28857#25968
            Width = 150
          end
          item
            Text = #21333#31080#24050#25195#25551#24635#32593#28857#25968
            Width = 150
          end
          item
            Text = #20449#23553#24050#25195#25551#24635#32593#28857#25968
            Width = 150
          end
          item
            Text = #22996#25910#24050#25195#25551#24635#32593#28857#25968
            Width = 150
          end
          item
            Text = #20449#23553#24050#25552#20132#24635#32593#28857#25968
            Width = 150
          end>
        SimplePanel = False
      end
    end
  end
  object MainMenuST: TMainMenu
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
      object R1: TMenuItem
        Caption = #21047#26032'[&R]'
        OnClick = R1Click
      end
    end
    object H1: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]'
      end
    end
  end
end
