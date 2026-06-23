
class GLBuffer
{
public:

	uint32 CreateBuffer()
	{
		std::cout << "Created Buffer\n";
		return 2;
	}
	uint32 DeleteBuffer()
	{
		std::cout << "Delete Buffer\n";
		return 3;
	}


	void Do()
	{
		std::cout << "Buffer: Do\n";
	}
};

class Buffer
{
public:

	Buffer() = default;
   ~Buffer() = default;


   template <typename T>
   void Init(const T& pT)
   {
	   //m_CreateBuffer = &pT.CreateBuffer;
	   //m_DeleteBuffer = &pT.DeleteBuffer;
   }

   uint32 CreateBuffer()
   {
	   return m_CreateBuffer();
   }

	uint32 DeleteBuffer()
	{
		return m_DeleteBuffer();
	}

private:
	uint32 (*m_CreateBuffer)() = nullptr; //Empty Buffer 
	uint32 (*m_DeleteBuffer)() = nullptr;

};