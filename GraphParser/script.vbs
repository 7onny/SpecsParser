Set objShell = CreateObject("Wscript.Shell")
objShell.CurrentDirectory = "C:\Users\Jonny\Projects\GraphParser\GraphParser"
objShell.run "cmd /K dot -Tbmp CEdiagram1.dot > CEdiagram1.bmp & dot -Tbmp CEdiagram2.dot > CEdiagram2.bmp & dot -Tbmp CEdiagram3.dot > CEdiagram3.bmp & dot -Tbmp CEdiagram4.dot > CEdiagram4.bmp & dot -Tbmp CEdiagram5.dot > CEdiagram5.bmp & dot -Tbmp CEdiagram6.dot > CEdiagram6.bmp & dot -Tbmp CEdiagram7.dot > CEdiagram7.bmp & dot -Tbmp CEdiagram8.dot > CEdiagram8.bmp & dot -Tbmp CEdiagram9.dot > CEdiagram9.bmp & exit"