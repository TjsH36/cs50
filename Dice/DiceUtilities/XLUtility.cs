using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Bytescout.Spreadsheet;

namespace Dice.DiceUtilities
{
    public class XLUtility
    {
        public static Spreadsheet xlfile = new Spreadsheet();
        //public static String filepath = "C:\\Share\\OldDeskTop\\Demo-Selinium\\Dice\\TestData\\States.xls";
        public static Worksheet mysheet;
        //static string bpath = System.IO.Directory.GetCurrentDirectory().Replace("\\bin\\Debug", "");
        //static String mypath = bpath.Replace("\\bin\\Debug", "");
        static string filepath = Path.Combine(System.IO.Directory.GetCurrentDirectory().Replace("\\bin\\Debug", ""), "TestData\\States.xls");
        public static Worksheet BringMySheet(String sheetname)
        {
            xlfile.LoadFromFile(filepath);
            mysheet = xlfile.Workbook.Worksheets.ByName(sheetname);
            return mysheet;
        }
        public static Worksheet AddMySheet(String sheetname)
        {
            xlfile.LoadFromFile(filepath);
            mysheet = xlfile.Workbook.Worksheets.Add(sheetname);
            xlfile.SaveAs(filepath);
            return mysheet;
        }


        public static void DeleteAllSheetsExceptMainSheet()
        {
            xlfile.LoadFromFile(filepath);
            int sheetCount = xlfile.Workbook.Worksheets.Count;
            int mySheetIndex = xlfile.Workbook.Worksheets.IndexOf("BooleanSearch");
            for (int i= sheetCount-1; i >= 0; i--)
                if(i != mySheetIndex)
                {
                    xlfile.Workbook.Worksheets.Delete(i);
                }
               
            xlfile.SaveAs(filepath);
        }
        public static int GetRowCount(String sheetname)
        {
            mysheet = BringMySheet(sheetname);
            int rc = -1;
            for (int i = 0; i <= 1000; i++)
            {
                if (mysheet.Cell(i, 0).Value == null)
                {
                    rc = i - 1;
                    break;
                }
            }
            return rc;
        }

        public static int GetColumnCount(String sheetname)
        {
            mysheet = BringMySheet(sheetname);
            int cc = -1;
            for (int i = 0; i <= 1000; i++)
            {
                if (mysheet.Cell(0, i).Value == null)
                {
                    cc = i;
                    break;
                }
            }
            return cc;
        }

        public static string ReadCellValue(String sheetname, int rownum, int colnum)
        {
            mysheet = BringMySheet(sheetname);

            return (mysheet.Cell(rownum, colnum).Value.ToString());
        }

        public static void WriteCellValue(String sheetname, int rownum, int colnum,String value)
        {
            mysheet = BringMySheet(sheetname);

            mysheet.Cell(rownum, colnum).Value = value ;
            xlfile.SaveAs(filepath);
            xlfile.Close();
        }

        public static string ReadCellValue(String sheetname, int rownum, String colname)
        {
            mysheet = BringMySheet(sheetname);
            int colindex = -1;
            //get the colindex
            for (int i = 0; i <= 1000; i++)
            {
                if (mysheet.Cell(0, i).Value.ToString().Equals(colname))
                {
                    colindex = i;
                    break;
                }
            }

            return (mysheet.Cell(rownum, colindex).Value.ToString());
        }

    }
}
