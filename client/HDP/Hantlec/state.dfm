object FormST: TFormST
  Left = 96
  Top = 11
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsDialog
  Caption = #28165#20998#29366#24577
  ClientHeight = 701
  ClientWidth = 1121
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
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1121
    Height = 701
    Align = alClient
    TabOrder = 0
    object Panel1: TPanel
      Left = 935
      Top = 1
      Width = 185
      Height = 699
      Align = alRight
      BevelOuter = bvLowered
      TabOrder = 0
      object Label1: TLabel
        Left = 24
        Top = 40
        Width = 40
        Height = 13
        Caption = #38454#27573'    '
      end
      object Label2: TLabel
        Left = 24
        Top = 80
        Width = 40
        Height = 13
        Caption = #29366#24577'    '
      end
      object cmbPhase: TComboBox
        Left = 64
        Top = 36
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
          #20449#23553#25552#20132
          '')
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
        Left = 63
        Top = 75
        Width = 89
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
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 934
      Height = 699
      Align = alClient
      BevelOuter = bvLowered
      Caption = 'Panel3'
      TabOrder = 1
      object txtDetail: TListView
        Left = 1
        Top = 1
        Width = 932
        Height = 669
        Align = alClient
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
        Top = 670
        Width = 932
        Height = 28
        Panels = <
          item
            Text = #32593#28857#24635#25968
            Width = 150
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
end
