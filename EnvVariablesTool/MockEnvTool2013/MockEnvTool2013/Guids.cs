// Guids.cs
// MUST match guids.h
using System;

namespace Microsoft.MockEnvTool2013
{
    static class GuidList
    {
        public const string guidMockEnvTool2013PkgString = "65645aaf-21af-41b0-846f-bdf2c5db1afd";
        public const string guidMockEnvTool2013CmdSetString = "1c0af022-8e61-4b64-b964-93b22327cbef";

        public static readonly Guid guidMockEnvTool2013CmdSet = new Guid(guidMockEnvTool2013CmdSetString);
    };
}