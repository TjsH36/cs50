using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium.Firefox;
using OpenQA.Selenium.IE;

namespace Dice.DiceUtilities
{
    public class TestSetUp
    {
        public static IWebDriver driver;
        public static String et = ConfigurationManager.AppSettings["ExecutionType"];
        public static String browser = ConfigurationManager.AppSettings["Browser"];
        public static IWebDriver BringMyDriver()
        {
            if (et.Equals("Local"))
            {
                return BringLocalDriver();
            }
            else if (et.Equals("Remote"))
            {
                return BringRemoteDriver();
            }
            else
            {
                return BringLocalDriver();
            }
        }

        public static IWebDriver BringLocalDriver()
        {
            if (browser.Equals("ie"))
            {
                driver = new InternetExplorerDriver();
                return driver;
            }
            else if (browser.Equals("chrome"))
            {
                driver = new ChromeDriver();
                return driver;
            }
            else
            {
                driver = new FirefoxDriver();
                return driver;
            }
        }

        public static IWebDriver BringRemoteDriver()
        {
            return new FirefoxDriver();

        }
    }
}
