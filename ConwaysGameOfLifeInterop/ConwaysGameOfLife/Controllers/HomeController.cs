using System;
using System.Linq;
using Microsoft.AspNetCore.Mvc;
using System.Runtime.InteropServices;

namespace ConwaysGameOfLife.Controllers
{    
    public class HomeController : Controller
    {
        [DllImport("Processing.dll", CallingConvention = CallingConvention.Cdecl)]
        extern static int GetWorldSize();

        [DllImport("Processing.dll", CallingConvention = CallingConvention.Cdecl)]
        extern static void GetRandomData(IntPtr data);

        [DllImport("Processing.dll", CallingConvention = CallingConvention.Cdecl)]
        extern static void GetNextStep(IntPtr requestData);

        [HttpGet]
        public IActionResult Index()
        {
            return View();
        }
        [HttpPost]
        public JsonResult GetRandomData()
        {
            int worldSize = GetWorldSize();
            int[] data = new int[worldSize];
            GCHandle handle = GCHandle.Alloc(data, GCHandleType.Pinned);
            try
            {
                IntPtr pointer = handle.AddrOfPinnedObject();
                GetRandomData(pointer);
                return new JsonResult(GetIntArray(pointer));
            }
            finally
            {
                if (handle.IsAllocated)
                {
                    handle.Free();
                }
            }
        }
        [HttpPost]
        public JsonResult NextStep()
        {
            if (Request.ContentLength == 0) return null;
            var requestData = Request.Form.ToList();
            int worldSize = GetWorldSize();
            int[] data = new int[worldSize];

            for(int i = 0; i < worldSize; i++)
            {
                data[i] = int.Parse(requestData[i].Value);
            }

            GCHandle handle = GCHandle.Alloc(data, GCHandleType.Pinned);
            try
            {
                IntPtr pointer = handle.AddrOfPinnedObject();
                int[] newData1 = GetIntArray(pointer);
                GetNextStep(pointer);
                int[] newData = GetIntArray(pointer);
                return new JsonResult(newData);                
            }
            finally
            {
                if (handle.IsAllocated)
                {
                    handle.Free();
                }
            }

            return null;
        }

        int[] GetIntArray(IntPtr data)
        {
            int worldSize = GetWorldSize();
            int[] result = new int[worldSize];
            Marshal.Copy(data, result, 0, worldSize);
            return result;
        }
    }
}
