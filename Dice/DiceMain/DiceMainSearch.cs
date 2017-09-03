using System;
using System.Configuration;
using System.Security.Policy;
using Dice.DicePages;
using Dice.DiceUtilities;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;

namespace Dice
{
    [TestClass]
    public class DwhDiceSearch
    {
        private static IWebDriver driver = TestSetUp.BringMyDriver();
        DiceHomePage myDiceHomePage = new DiceHomePage(driver);
        [TestInitialize]
        public void LaunchApplication()
        {
            XLUtility.DeleteAllSheetsExceptMainSheet();
            //driver.Manage().Window.Maximize();
            driver.Navigate().GoToUrl(ConfigurationManager.AppSettings["AppURL"]);
        }
        [TestMethod]
        public void Dice()
        {

        }
        [TestMethod]
        public void DiceBooleanSearch()
        {
            myDiceHomePage.BooleanSearch();
        }
    }
}
