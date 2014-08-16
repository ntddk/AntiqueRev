using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            var s = new string("ExitProcess".Select(c => (char)(c - 2)).ToArray())
            ;
        }
    }
}
