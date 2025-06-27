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

		self.main_backgroudn = ui.ExpandedImageBox()
		self.main_backgroudn.SetParent(self)
		self.main_backgroudn.LoadImage("d:/ymir work/ui/tournament/background.tga")
		self.main_backgroudn.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2, - 5)
		self.main_backgroudn.SetScale(0.9, 0.9)
		self.main_backgroudn.Show()
		
		self.main_text = ui.TextLine()
		
		self.main_text.online_A = ui.TextLine()
		self.main_text.online_A.SetParent(self)
		self.main_text.online_A.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 240, 27)
		self.main_text.online_A.SetFontName("Roman:20")
		self.main_text.online_A.SetPackedFontColor(0xff948b7d)
		self.main_text.online_A.SetHorizontalAlignLeft()
		self.main_text.online_A.Show()
		
		self.main_text.online_B = ui.TextLine()
		self.main_text.online_B.SetParent(self)
		self.main_text.online_B.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 500, 27)
		self.main_text.online_B.SetFontName("Roman:20")
		self.main_text.online_B.SetPackedFontColor(0xff948b7d)
		self.main_text.online_B.SetHorizontalAlignLeft()
		self.main_text.online_B.Show()
		
		self.main_text.membersDeadA = ui.TextLine()
		self.main_text.membersDeadA.SetParent(self)
		self.main_text.membersDeadA.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 180, 22)
		self.main_text.membersDeadA.SetFontName("Roman:20")
		self.main_text.membersDeadA.SetPackedFontColor(0xff948b7d)
		self.main_text.membersDeadA.SetHorizontalAlignLeft()
		self.main_text.membersDeadA.Show()
		
		self.main_text.membersDeadB = ui.TextLine()
		self.main_text.membersDeadB.SetParent(self)
		self.main_text.membersDeadB.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 565, 22)
		self.main_text.membersDeadB.SetFontName("Roman:20")
		self.main_text.membersDeadB.SetPackedFontColor(0xff948b7d)
		self.main_text.membersDeadB.SetHorizontalAlignLeft()
		self.main_text.membersDeadB.Show()
		
		self.main_text.memberLives = ui.TextLine()
		self.main_text.memberLives.SetParent(self)
		self.main_text.memberLives.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 374, 80)
		self.main_text.memberLives.SetFontName("Roman:20")
		self.main_text.memberLives.SetPackedFontColor(0xffff4719)
		self.main_text.memberLives.SetHorizontalAlignLeft()
		self.main_text.memberLives.Show()
		
		self.main_text.clock = ui.TextLine()
		self.main_text.clock.SetParent(self)
		self.main_text.clock.SetPosition(wndMgr.GetScreenWidth()/2 - 750 / 2 + 350, 22)
		self.main_text.clock.SetFontName("Roman:25")
		self.main_text.clock.SetPackedFontColor(0xff948b7d)
		self.main_text.clock.SetHorizontalAlignLeft()
		self.main_text.clock.Show()
		
	def Append(self, tokens):
		if constInfo.TOURNAMENT_WINDOW_IS_SHOWED < 1:
			# Establecer el tiempo de finalización del torneo
			self.leftTime = app.GetGlobalTimeStamp() + int(tokens[0])
	
		self.Show()
		
		# Actualizar los datos de la interfaz
		self.main_text.online_A.SetText(tokens[1])
		self.main_text.online_B.SetText(tokens[2])		
		self.main_text.membersDeadA.SetText(tokens[3])
		self.main_text.membersDeadB.SetText(tokens[4])
		self.main_text.memberLives.SetText(tokens[5])
		constInfo.TOURNAMENT_WINDOW_IS_SHOWED = 1

	def SetTime(self, iLeft):
		leftTime = iLeft - app.GetGlobalTimeStamp()
		
		# Asegurarse de que el tiempo no sea negativo
		if leftTime < 0:
			leftTime = 0
			self.Hide()  # Ocultar la interfaz si el tiempo restante es 0
	
		# Actualizar el texto del reloj
		self.main_text.clock.SetText(("%s" % (CalculateTimeLeft(leftTime))))

	def OnUpdate(self):
		self.SetTime(int(self.leftTime))

	def Destroy(self):
		self.ClearDictionary()