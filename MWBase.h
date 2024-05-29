#pragma once

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
class CMWBase
{
	public:
		CMWBase(void);
		virtual ~CMWBase(void)  = 0;  // virtual destructors are necessary

		protected:
			static CRuntimeClass* PASCAL _GetBaseClass();

		public:
			BOOL IsSerializable() const;
			BOOL IsKindOf(const CRuntimeClass* pClass) const;

			static const CRuntimeClass classCMWBase;
			static CRuntimeClass* PASCAL GetThisClass();
			virtual CRuntimeClass* GetRuntimeClass() const;
};

