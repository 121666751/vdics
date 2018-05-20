object FmSethosts: TFmSethosts
  Left = 373
  Top = 310
  Width = 544
  Height = 400
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #32593#32476#21442#25968#35774#32622
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  Icon.Data = {
    0000010002002020100000000000E80200002600000010101000000000002801
    00000E0300002800000020000000400000000100040000000000800200000000
    0000000000000000000000000000000000000000800000800000008080008000
    0000800080008080000080808000C0C0C0000000FF0000FF000000FFFF00FF00
    0000FF00FF00FFFF0000FFFFFF00000000000000000000000000000000000000
    00000000000000000000000000000700000000000000F8888888888870000070
    0000000000000000000000000000000700000000000000F88888887000000000
    70000000000000F8888888700000000007000000000000F87878787000000000
    00700000000000F7A7B797700000000000070000000000F87878787000000000
    00007000000000F8888888700000000000000700000000F888887F7000000000
    00000070000000F7FFFF77700000000000000007000000F777788870000000F7
    77777700700000F7FFFF8870000000F777777770070000F777788870000000F7
    77777777007000F888888870000000FFFFFFFFFFF0070007FFFFF87000000000
    00000000000070007777787000000000000000000000070008888870000000F8
    8888888888880070008FF870000000F8777777777777800700077870000000F8
    0CCCC99999C7800070008870000000F80CCCCC9999C7800007000870000000F8
    0CCCCCC999C7800000700007000000F80ECCCCBB99C7800000070000000000F8
    0EECCBBBB9C7800000007000000000F80EEEBBBBBBC7800000000700000000F8
    000000000007800000000070000000F8888888888888800000000007000000FF
    FFFFFFFFFFFFF000000000007000000000000000000000000000000007000000
    0000000000000000000000000000FFFFFFFFFFFC00019FFE0003CFFF0007E7FF
    800FF3FF800FF9FF800FFCFF800FFE7F800FFF3F800FFF9F800FFFCF800FC067
    800F8033800F8019800F800CC00F8006600FC003300FC001980F8000CC0F8000
    660F8000330F8000398F80003CCF80003E7F80003F3F80003F9F80003FCF8000
    3FE780003FF3C0007FFBFFFFFFFF280000001000000020000000010004000000
    0000C00000000000000000000000000000000000000000000000000080000080
    00000080800080000000800080008080000080808000C0C0C0000000FF0000FF
    000000FFFF00FF000000FF00FF00FFFF0000FFFFFF0000000000000000000000
    0000000000000700000070F880700070000000F880000007000000FA80000000
    700000F88000000007000087700000F8807000F8800000F8880700F770000000
    00007008800000F888800700070000FCC9800070000000FEBB800007000000FF
    FFF00000700000000000000007000000000000000000FFFF0000FF0100009F01
    0000CF830000E7830000F3830000C98300008483000082430000C12300008093
    000080CF000080E7000080F30000C1FB0000FFFF0000}
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  Visible = True
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Bevel2: TBevel
    Left = 248
    Top = 8
    Width = 280
    Height = 314
    Shape = bsFrame
    Style = bsRaised
  end
  object Bevel1: TBevel
    Left = 8
    Top = 320
    Width = 248
    Height = 2
    Shape = bsFrame
    Style = bsRaised
  end
  object Bevel3: TBevel
    Left = 80
    Top = 8
    Width = 180
    Height = 2
    Shape = bsFrame
    Style = bsRaised
  end
  object Label10: TLabel
    Left = 16
    Top = 0
    Width = 60
    Height = 20
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Hosts'#25991#20214
    Layout = tlCenter
  end
  object Label11: TLabel
    Left = 12
    Top = 324
    Width = 189
    Height = 20
    AutoSize = False
    Caption = #21271#20140#22825#21019#24449#33150#20449#24687#31185#25216#26377#38480#20844#21496'   '
    Enabled = False
  end
  object Label12: TLabel
    Left = 276
    Top = 100
    Width = 110
    Height = 20
    Alignment = taRightJustify
    AutoSize = False
    Caption = #28040#24687#35843#24230#26381#21153' IP'#65306
    Layout = tlCenter
  end
  object Label13: TLabel
    Left = 269
    Top = 128
    Width = 110
    Height = 20
    Alignment = taRightJustify
    AutoSize = False
    Caption = '(MESSAGE_SERVER)'
    Enabled = False
  end
  object Label1: TLabel
    Left = 276
    Top = 42
    Width = 110
    Height = 20
    Alignment = taRightJustify
    AutoSize = False
    Caption = #25968#25454#24211#26381#21153#22120'IP'#65306
    Layout = tlCenter
  end
  object Label3: TLabel
    Left = 267
    Top = 64
    Width = 110
    Height = 20
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'ZTIC_DB_SERVER)'
    Enabled = False
  end
  object btnSaveHosts: TButton
    Left = 199
    Top = 335
    Width = 100
    Height = 25
    Caption = #20445'  '#23384
    TabOrder = 1
    OnClick = btnSaveHostsClick
  end
  object btnRefresh: TButton
    Left = 309
    Top = 335
    Width = 100
    Height = 25
    Caption = #24674'  '#22797
    TabOrder = 2
    OnClick = btnRefreshClick
  end
  object btnExit: TButton
    Left = 419
    Top = 335
    Width = 100
    Height = 25
    Caption = #36864'  '#20986
    Default = True
    ModalResult = 2
    TabOrder = 3
    OnClick = btnExitClick
  end
  object lstHost: TMemo
    Left = 8
    Top = 20
    Width = 225
    Height = 295
    TabStop = False
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 4
  end
  object message_server: TEdit
    Left = 396
    Top = 100
    Width = 120
    Height = 20
    AutoSize = False
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    TabOrder = 0
    OnChange = local_db_hostChange
  end
  object ztic_db_server: TEdit
    Left = 396
    Top = 42
    Width = 120
    Height = 20
    AutoSize = False
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    TabOrder = 5
    OnChange = local_db_hostChange
  end
end
