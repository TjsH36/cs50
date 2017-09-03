using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Dice.DiceUtilities;
using OpenQA.Selenium;
using OpenQA.Selenium.Support.PageObjects;

namespace Dice.DicePages
{
    public class DiceHomePage
    {
        IWebDriver driver;

        public DiceHomePage(IWebDriver globaldriver)
        {
            this.driver = globaldriver;
            PageFactory.InitElements(driver, this);
            driver.Manage().Timeouts().ImplicitlyWait(TimeSpan.FromSeconds(20));
        }
        [FindsBy(How = How.Id, Using = "job")]
        public IWebElement SearchField;
        [FindsBy(How = How.Id, Using = "location")]
        public IWebElement SearchLocation;
        [FindsBy(How = How.CssSelector, Using = ".error-page-header")]
        public IWebElement NoResults;
        [FindsBy(How = How.ClassName, Using = "jobs-page-header")]
        public IWebElement ResultsFound;
        [FindsBy(How = How.Id, Using = "posiCountId")]
        public IWebElement PositionsCount;
        [FindsBy(How = How.ClassName, Using = "icon-filled-arrow-66")]
        public IWebElement NextPage;
        [FindsBy(How = How.XPath, Using = "//a[contains(@title,'Go to page 2')]")]
        public IWebElement GoToPage2;
        [FindsBy(How = How.XPath, Using = "//a[contains(@title,'Go to page 3')]")]
        public IWebElement GoToPage3;
        [FindsBy(How = How.XPath, Using = "//a[contains(@title,'Go to page 4')]")]
        public IWebElement GoToPage4;

        public void DiceSearch(String searchKey, String city)
        {
            for (int i = 1; i < 10; i++)
            {
                string ct = XLUtility.ReadCellValue("WA", i, 2);
                string st = XLUtility.ReadCellValue("WA", i, 4);
                string newcs = ct + "," + st;
                SearchField.Clear();
                SearchField.SendKeys(searchKey);
                SearchLocation.Clear();
                SearchLocation.SendKeys(newcs);
                SelectCheckBox("Contracts");
                SelectCheckBox("Third Party");
                driver.FindElements(By.CssSelector(".btn-primary")).ElementAt(0).Click();
            }
        }
        public void BooleanSearch()
        {
            SelectCheckBox("Contracts");
            SelectCheckBox("Third Party");
            for (int i = 1; i < 5; i++)
            {
                string searchString = XLUtility.ReadCellValue("BooleanSearch", i, "SearchKey");
                runSearch(searchString);
                String newSheetName = XLUtility.ReadCellValue("BooleanSearch", i, "CandidateName");
                XLUtility.AddMySheet(newSheetName);
                writeColumTitles(newSheetName);
                CaptureTheSearchResults(newSheetName, searchString);
            }
        }
        public void writeColumTitles(String newSheetName)
        {
            XLUtility.WriteCellValue(newSheetName, 0, 0, "SearchKey");
            XLUtility.WriteCellValue(newSheetName, 0, 1, "JobTitle");
            XLUtility.WriteCellValue(newSheetName, 0, 2, "URL");
            XLUtility.WriteCellValue(newSheetName, 0, 3, "CompanyName");
            XLUtility.WriteCellValue(newSheetName, 0, 4, "Location");
            XLUtility.WriteCellValue(newSheetName, 0, 5, "PostedDate");
            XLUtility.WriteCellValue(newSheetName, 0, 6, "PostedDate");
        }

        public void CaptureTheSearchResults(String sheetName, String searchString)
        {
            try
            {
                if (driver.FindElements(By.CssSelector(".container")).ElementAt(2).Text.Contains("No jobs found for Contracts OR Third Party"))
                {
                    ReadOnlyCollection<IWebElement> noResultsList = driver.FindElements(By.CssSelector(".error-page-header"));
                    Debug.WriteLine(noResultsList.ElementAt(0).Text);
                    XLUtility.WriteCellValue(sheetName, 1, 0, searchString);
                    XLUtility.WriteCellValue(sheetName, 1, 1, "No Results Found");
                }
                else
                {
                    ReadOnlyCollection<IWebElement> results = driver.FindElements(By.CssSelector(".posiCount "));
                    Debug.WriteLine(results.ElementAt(1).Text);
                    //if result count >30,>60,>90
                    int intialValue = 0;
                    int i = 0;
                    //int pageCount = GetPageCount();
                    for (int j = 0; j < 3; j++)
                    {
                        for (i = intialValue; i <= (intialValue + 15); i++)
                        {
                            ReadJobPositionDetailsAndWriteToXl(sheetName,i, searchString);
                        }
                        NextPage.Click();
                        intialValue = i;
                    }

                }
            }
            catch (Exception e)
            {
                Debug.WriteLine("No Results Found ");
            }
        }
        public void CaptureTheSearchResults(int sheetIndex, String searchKey)
        {
            Thread.Sleep(5000);
            try
            {
                if (driver.FindElements(By.CssSelector(".container")).ElementAt(2).Text.Contains("No jobs found for Contracts OR Third Party"))
                {
                    ReadOnlyCollection<IWebElement> noResultsList = driver.FindElements(By.CssSelector(".error-page-header"));
                    Debug.WriteLine(noResultsList.ElementAt(0).Text);
                    XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, 1, 0, searchKey);
                    XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, 1, 1, "No Results Found");
                }
                else
                {
                    ReadOnlyCollection<IWebElement> results = driver.FindElements(By.CssSelector(".posiCount "));
                    Debug.WriteLine(results.ElementAt(1).Text);
                    //if result count >30,>60,>90
                    int intialValue = 0;
                    int i = 0;
                    int pageCount = GetPageCount();
                    for(int j=0;j<pageCount;j++)
                    {
                        for (i = intialValue; i <= (intialValue+5); i++)
                        {
                            Thread.Sleep(3000);
                            ReadJobPositionDetailsAndWriteToXl(i, sheetIndex, searchKey);
                        }
                        NextPage.Click();
                        intialValue = i;
                    }
                        
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine("No Results Found ");
            }
        }
        public int GetPageCount()
        {
            int pages = 0;
            int pc =   Int32.Parse(PositionsCount.Text);
            if(pc<30)
            {
                pages = 1; 
            }
            else if(pc>30 && pc < 60)
            {
                pages = 2;
            }
            else if(pc > 60 && pc < 90)
            {
                pages = 3;
            }
            else if (pc > 90 )
            {
                pages = 4;
            }
            return pages;
        }
        public void runSearch(String searchString)
        {
            SearchField.Clear();
            Thread.Sleep(1000);
            SearchField.SendKeys(searchString);
            SearchLocation.Clear();
            driver.FindElements(By.CssSelector(".btn-primary")).ElementAt(0).Click();
            Thread.Sleep(1000);
        }
        public void ReadJobPositionDetailsAndWriteToXl(String sheetName,int position, String searchString)
        {
            //Thread.Sleep(5000);
            IWebElement myJobPosition = driver.FindElement(By.Id("position" + position));
            String jobTitle = myJobPosition.GetAttribute("title");
            String url = myJobPosition.GetAttribute("href");
            IWebElement company = driver.FindElement(By.Id("company" + position));
            String companyName = company.GetAttribute("text");
            ReadOnlyCollection<IWebElement> locations = driver.FindElements(By.CssSelector(".location"));
            String location = locations.ElementAt(position).Text;
            ReadOnlyCollection<IWebElement> postedOn = driver.FindElements(By.CssSelector(".posted"));
            String postedDate = postedOn.ElementAt(position).Text;
           // Thread.Sleep(2000);
            XLUtility.WriteCellValue(sheetName, position + 1, 0, searchString);
            XLUtility.WriteCellValue(sheetName, position + 1, 1, jobTitle);
            XLUtility.WriteCellValue(sheetName, position + 1, 2, url);
            XLUtility.WriteCellValue(sheetName, position + 1, 3, companyName);
            XLUtility.WriteCellValue(sheetName, position + 1, 4, location);
            XLUtility.WriteCellValue(sheetName, position + 1, 5, postedDate);
        }

        public void ReadJobPositionDetailsAndWriteToXl(int position, int sheetIndex, String searchKey)
        {
            IWebElement myJobPosition = driver.FindElement(By.Id("position" + position));
            String jobTitle = myJobPosition.GetAttribute("title");
            String url = myJobPosition.GetAttribute("href");
            IWebElement company = driver.FindElement(By.Id("company" + position));
            String companyName = company.GetAttribute("text");
            ReadOnlyCollection<IWebElement> locations = driver.FindElements(By.CssSelector(".location"));
            String location = locations.ElementAt(position).Text;
            ReadOnlyCollection<IWebElement> postedOn = driver.FindElements(By.CssSelector(".posted"));
            String postedDate = postedOn.ElementAt(position).Text;
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 0, searchKey);
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 1, jobTitle);
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 2, url);
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 3, companyName);
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 4, location);
            XLUtility.WriteCellValue("BooleanSearch" + sheetIndex, position + 1, 5, postedDate);
        }

        public void SelectCheckBox(String chkName)
        {
            ReadOnlyCollection<IWebElement> chkCollection = driver.FindElements(By.ClassName("typeChkBox"));

            foreach (var checkbox in chkCollection)
            {
                if (checkbox.GetAttribute("value").ToLower().Equals(chkName.ToLower()))
                {
                    checkbox.Click();
                    break;
                }
            }
        }
    }
}

