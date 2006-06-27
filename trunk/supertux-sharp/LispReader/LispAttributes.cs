using System;

namespace LispReader
{

	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct,
	                AllowMultiple=false)]
	public class LispRootAttribute : Attribute
	{
		public string Name;
		
		public LispRootAttribute(string name)
		{
			this.Name = name;
		}
	}

	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct,
	                AllowMultiple=false)]
	public class LispCustomSerializerAttribute : Attribute
	{
		public Type Type;
		
		public LispCustomSerializerAttribute(Type type)
		{
			this.Type = type;
		}
	}

	[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property,
	                AllowMultiple=false)]
	public class LispChildAttribute : Attribute
	{
		public bool Translatable;
		public bool Optional;
		public object Default;
		public string Name;

		public LispChildAttribute(string Name) {
			this.Name = Name;
		}
	}

	[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property,
	                AllowMultiple=true)]
	public class LispChildsAttribute : Attribute
	{
		public string Name;
		public Type Type;
		private Type _listType;

		public Type ListType {
			set {
				_listType = value;
			}
			get {
				if(_listType == null)
					return Type;
				else
					return _listType;
			}
		}
	}

}