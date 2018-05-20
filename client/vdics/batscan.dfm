object FormScan: TFormScan
  Left = 244
  Top = 8
  Width = 1024
  Height = 738
  Caption = #25195#25551#25511#21046
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1008
    Height = 680
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 0
    object statusBar: TStatusBar
      Left = 1
      Top = 651
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
          Width = 150
        end
        item
          Width = 300
        end>
      SimplePanel = False
    end
    object ToolBar1: TToolBar
      Left = 1
      Top = 1
      Width = 1006
      Height = 29
      Caption = 'ToolBar1'
      TabOrder = 1
    end
    object Panel2: TPanel
      Left = 1
      Top = 30
      Width = 1006
      Height = 621
      Align = alClient
      BevelOuter = bvNone
      Caption = 'Panel2'
      TabOrder = 2
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 225
        Height = 621
        Align = alLeft
        BevelOuter = bvNone
        Caption = 'Panel3'
        TabOrder = 0
        object txtFnameList: TListView
          Left = 0
          Top = 0
          Width = 225
          Height = 621
          Align = alClient
          Columns = <
            item
              Caption = #25991#20214#21517
              Width = 200
            end>
          ReadOnly = True
          PopupMenu = popMenu
          TabOrder = 0
          ViewStyle = vsReport
          OnSelectItem = txtFnameListSelectItem
        end
      end
      object Panel4: TPanel
        Left = 821
        Top = 0
        Width = 185
        Height = 621
        Align = alRight
        BevelOuter = bvLowered
        TabOrder = 1
        object GroupBox1: TGroupBox
          Left = 8
          Top = 16
          Width = 169
          Height = 105
          TabOrder = 0
          object Label2: TLabel
            Left = 8
            Top = 25
            Width = 39
            Height = 13
            Caption = #26085#26399'     '
          end
          object Label3: TLabel
            Left = 9
            Top = 65
            Width = 36
            Height = 13
            Caption = #22330#27425'    '
          end
          object txtDate: TDateTimePicker
            Left = 48
            Top = 20
            Width = 105
            Height = 21
            Hint = #26085#26399
            CalAlignment = dtaLeft
            Date = 37764.4105923843
            Time = 37764.4105923843
            Color = clBtnFace
            DateFormat = dfShort
            DateMode = dmComboBox
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = #23435#20307
            Font.Style = [fsBold]
            ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
            Kind = dtkDate
            ParseInput = False
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
          end
          object txtSession: TEdit
            Left = 48
            Top = 65
            Width = 89
            Height = 24
            Color = clMenu
            Font.Charset = GB2312_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = #23435#20307
            Font.Style = []
            ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
            ParentFont = False
            TabOrder = 1
            Text = '1'
          end
          object UpDown: TUpDown
            Left = 137
            Top = 65
            Width = 16
            Height = 24
            Associate = txtSession
            Min = 1
            Max = 3
            Position = 1
            TabOrder = 2
            Wrap = False
          end
        end
        object GroupBox2: TGroupBox
          Left = 9
          Top = 264
          Width = 169
          Height = 337
          TabOrder = 1
          object btnStart: TButton
            Left = 35
            Top = 47
            Width = 97
            Height = 41
            Caption = #24320'  '#22987' &A'
            TabOrder = 0
            OnClick = btnStartClick
          end
          object btnStop: TButton
            Left = 34
            Top = 101
            Width = 97
            Height = 41
            Caption = #23436'   '#25104' &S'
            Enabled = False
            TabOrder = 1
            OnClick = btnStopClick
          end
          object btnCancel: TButton
            Left = 34
            Top = 160
            Width = 97
            Height = 41
            Caption = #25918'   '#24323' &C'
            Enabled = False
            TabOrder = 2
            OnClick = btnCancelClick
          end
          object btnExit: TButton
            Left = 34
            Top = 264
            Width = 97
            Height = 41
            Caption = #36864'    '#20986' &X'
            TabOrder = 3
            OnClick = btnExitClick
          end
          object Rotate: TButton
            Left = 36
            Top = 213
            Width = 97
            Height = 41
            Caption = #26059'   '#36716' &T'
            Enabled = False
            TabOrder = 4
            OnClick = RotateClick
          end
        end
        object chkScan: TCheckBox
          Left = 16
          Top = 136
          Width = 137
          Height = 17
          Caption = #26174#31034#25195#25551#36807#31243'      '
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object chbShow: TCheckBox
          Left = 16
          Top = 174
          Width = 113
          Height = 17
          Caption = #26174#31034'Twain'#30028#38754
          TabOrder = 3
        end
      end
      object Panel5: TPanel
        Left = 225
        Top = 0
        Width = 596
        Height = 621
        Align = alClient
        BevelOuter = bvLowered
        Caption = 'Panel5'
        TabOrder = 2
        object Panel6: TPanel
          Left = 1
          Top = 1
          Width = 594
          Height = 264
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object imgF: TImage
            Left = 0
            Top = 0
            Width = 594
            Height = 264
            Align = alClient
            Proportional = True
            Stretch = True
            OnMouseUp = imgFMouseUp
          end
        end
        object Panel7: TPanel
          Left = 1
          Top = 265
          Width = 594
          Height = 355
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object imgB: TImage
            Left = 0
            Top = 0
            Width = 594
            Height = 355
            Align = alClient
            Proportional = True
            Stretch = True
            OnMouseUp = imgBMouseUp
          end
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 312
    Top = 29
    object muFile: TMenuItem
      Caption = #25991#20214'[&F]'
      object muExit: TMenuItem
        Caption = #36864#20986'[&X]'
        ShortCut = 32883
        OnClick = muExitClick
      end
    end
    object S1: TMenuItem
      Caption = #35774#32622'[&S]'
      object muSysSetup: TMenuItem
        Caption = #31995#32479#35774#32622'...'
        OnClick = muSysSetupClick
      end
    end
    object H1: TMenuItem
      Caption = #24110#21161'[&H]'
      object A1: TMenuItem
        Caption = #20851#20110'[&A]...'
      end
    end
  end
  object popMenu: TPopupMenu
    Left = 480
    Top = 384
    object F1: TMenuItem
      Caption = #21024#38500#22270#20687' &D'
      OnClick = F1Click
    end
  end
end
