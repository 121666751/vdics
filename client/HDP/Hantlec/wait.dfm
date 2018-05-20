object FmWait: TFmWait
  Left = 393
  Top = 337
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = #31561#24453
  ClientHeight = 80
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clNavy
  Font.Height = -16
  Font.Name = #23435#20307
  Font.Style = [fsBold]
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 16
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 320
    Height = 80
    Align = alClient
    Shape = bsFrame
  end
  object txtMessage: TLabel
    Left = 40
    Top = 32
    Width = 243
    Height = 16
    Caption = '    '#27491#22312#21021#22987#21270#65292#35831#31245#21518'......'
  end
  object Animate1: TAnimate
    Left = 36
    Top = 32
    Width = 16
    Height = 16
    Active = True
    CommonAVI = aviFindComputer
    StopFrame = 8
  end
end
