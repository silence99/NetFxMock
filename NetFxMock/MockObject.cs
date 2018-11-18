using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;

namespace NetFxMock
{
	public class MockObject
	{
		private static readonly object NullObject = new object();
		private Dictionary<MethodInfo, Dictionary<object, MethodInfo>> _mockedMethods = new Dictionary<MethodInfo, Dictionary<object, MethodInfo>>();

		public MethodInfo GetMocked(object instance, MethodInfo method)
		{
			if (method == null)
			{
				return null;
			}

			if (!_mockedMethods.ContainsKey(method))
			{
				return null;
			}

			if (method.IsStatic)
			{
				return _mockedMethods[method].ContainsKey(NullObject) ? _mockedMethods[method][NullObject] : null;
			}

			if (instance != null)
			{
				return _mockedMethods[method].ContainsKey(instance) ?
					_mockedMethods[method][instance] :
					(_mockedMethods[method].ContainsKey(NullObject) ? _mockedMethods[method][NullObject] : null);
			}
			else
			{
				return _mockedMethods[method].ContainsKey(NullObject) ? _mockedMethods[method][NullObject] : null;
			}
		}

		public bool IsMocked(object instance, MethodInfo method)
		{
			return GetMocked(instance, method) != null;
		}

		public static byte[] GetCompileMethodMockFragment()
		{
			var method = new DynamicMethod("MockFragment", typeof(object), null);
			var il = method.GetILGenerator();
			var currentMethodInfo = il.DeclareLocal(typeof(MethodInfo));
			var instance = il.DeclareLocal(typeof(object));
			var args = il.DeclareLocal(typeof(object[]));
			var labelNotStatic = il.DefineLabel();
			var labelNotStaticEnd = il.DefineLabel();
			var labelIsMockedEnd = il.DefineLabel();

			il.Emit(OpCodes.Nop);
			var currentMethod = typeof(MethodBase).GetMethod("GetCurrentMethod");
			var getMockedMethod = typeof(MockObject).GetMethod("GetMocked");
			var isStaticMethod = typeof(MethodInfo).GetProperty("IsStatic").GetMethod;
			var invokeMethod = typeof(MethodInfo).GetMethod("Invoke", new Type[] { typeof(object), typeof(object[]) });

			// set current method info to loc 0
			il.Emit(OpCodes.Call, currentMethod);
			il.Emit(OpCodes.Stloc_0);

			// set instance to loc1
			il.Emit(OpCodes.Ldloc_0);
			il.Emit(OpCodes.Call, isStaticMethod);
			il.Emit(OpCodes.Brfalse, labelNotStatic);
			il.Emit(OpCodes.Ldnull);
			il.Emit(OpCodes.Br_S, labelNotStaticEnd);
			il.MarkLabel(labelNotStatic);
			il.Emit(OpCodes.Ldarg_0);
			il.MarkLabel(labelNotStaticEnd);
			il.Emit(OpCodes.Stloc_1);

			// call is mocked
			il.Emit(OpCodes.Ldloc_1);
			il.Emit(OpCodes.Ldloc_0);
			il.Emit(OpCodes.Call, getMockedMethod);
			il.Emit(OpCodes.Ldnull);
			il.Emit(OpCodes.Ceq);
			il.Emit(OpCodes.Brtrue_S, labelIsMockedEnd); //if null, call orignal function flow

			// jump if mocked


			return null;
		}
	}
}
