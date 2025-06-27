import chat
import grp
import net
import app
import wndMgr
import uiCommon
import uiGuild
import uiToolTip
import ui
import constInfo
import locale

def CalculateTimeLeft(iTime):
	A, B = divmod(iTime, 60)
	C, A = divmod(A, 60)
	return "%02d:%02d" % (A, B)	

class Window(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)

	def __del__(self):
		ui.Window.__del__(self)

	def Initialize(self):
		self.main = {}

		self.main["background"] = ui.ExpandedImageBox()
		self.main["background"].SetParent(self)
		self.main["background"].LoadImage("d:/ymir work/ui/tournament/background.tga")
		self.main["background"].SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2, - 5)
		self.main["background"].SetScale(0.9, 0.9)
		self.main["background"].Show()
		
		self.main["text"] = ui.TextLine()
		
		self.main["text"].online_A = ui.TextLine()
		self.main["text"].online_A.SetParent(self)
		self.main["text"].online_A.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 240, 27)
		self.main["text"].online_A.SetFontName("Roman:20")
		self.main["text"].online_A.SetPackedFontColor(0xff948b7d)
		self.main["text"].online_A.SetHorizontalAlignLeft()
		self.main["text"].online_A.Show()
		
		self.main["text"].online_B = ui.TextLine()
		self.main["text"].online_B.SetParent(self)
		self.main["text"].online_B.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 500, 27)
		self.main["text"].online_B.SetFontName("Roman:20")
		self.main["text"].online_B.SetPackedFontColor(0xff948b7d)
		self.main["text"].online_B.SetHorizontalAlignLeft()
		self.main["text"].online_B.Show()
		
		self.main["text"].membersDeadA = ui.TextLine()
		self.main["text"].membersDeadA.SetParent(self)
		self.main["text"].membersDeadA.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 180, 22)
		self.main["text"].membersDeadA.SetFontName("Roman:20")
		self.main["text"].membersDeadA.SetPackedFontColor(0xff948b7d)
		self.main["text"].membersDeadA.SetHorizontalAlignLeft()
		self.main["text"].membersDeadA.Show()
		
		self.main["text"].membersDeadB = ui.TextLine()
		self.main["text"].membersDeadB.SetParent(self)
		self.main["text"].membersDeadB.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 565, 22)
		self.main["text"].membersDeadB.SetFontName("Roman:20")
		self.main["text"].membersDeadB.SetPackedFontColor(0xff948b7d)
		self.main["text"].membersDeadB.SetHorizontalAlignLeft()
		self.main["text"].membersDeadB.Show()
		
		self.main["text"].memberLives = ui.TextLine()
		self.main["text"].memberLives.SetParent(self)
		self.main["text"].memberLives.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 374, 80)
		self.main["text"].memberLives.SetFontName("Roman:20")
		self.main["text"].memberLives.SetPackedFontColor(0xffff4719)
		self.main["text"].memberLives.SetHorizontalAlignLeft()
		self.main["text"].memberLives.Show()
		
		self.main["text"].clock = ui.TextLine()
		self.main["text"].clock.SetParent(self)
		self.main["text"].clock.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 350, 22)
		self.main["text"].clock.SetFontName("Roman:25")
		self.main["text"].clock.SetPackedFontColor(0xff948b7d)
		self.main["text"].clock.SetHorizontalAlignLeft()
		self.main["text"].clock.Show()
		
	def Append(self, tokens):
		if constInfo.TOURNAMENT_WINDOW_IS_SHOWED < 1:
			self.leftTime = app.GetGlobalTimeStamp() + int(tokens[0])

		self.Show()
		
		self.main["text"].online_A.SetText(tokens[1])
		self.main["text"].online_B.SetText(tokens[2])		
		self.main["text"].membersDeadA.SetText(tokens[3])
		self.main["text"].membersDeadB.SetText(tokens[4])
		self.main["text"].memberLives.SetText(tokens[5])
		constInfo.TOURNAMENT_WINDOW_IS_SHOWED = 1

	def SetTime(self, iLeft):
		leftTime = iLeft - app.GetGlobalTimeStamp()
		
		if leftTime <= 0:
			leftTime = 0
			self.Hide()

		self.main["text"].clock.SetText(("%s" % (CalculateTimeLeft(leftTime))))

	def OnUpdate(self):
		self.SetTime(int(self.leftTime))

	def Destroy(self):
		self.ClearDictionary()