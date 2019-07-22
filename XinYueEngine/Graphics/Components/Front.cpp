///////////////////////////////////////////////////////////////////////////////
// Filename: Front.cpp
///////////////////////////////////////////////////////////////////////////////
#include "Front.h"


Front::Front()
{
	m_Front = 0;
	m_Texture = 0;
}


Front::Front(const Front& other)
{
}


Front::~Front()
{
}


bool Front::Initialize(ID3D11Device* device, const char* FrontFilename, const char* textureFilename)
{
	bool result;


	// Load in the text file containing the Front data.
	result = LoadFrontData(FrontFilename);
	if(!result)
	{
		return false;
	}

	// Load the texture that has the Front characters on it.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Front::Shutdown()
{
	// Release the Front texture.
	ReleaseTexture();

	// Release the Front data.
	ReleaseFrontData();

	return;
}


bool Front::LoadFrontData(const char* filename)
{
	ifstream fin;
	int i;
	char temp;


	// Create the Front spacing buffer.
	m_Front = new FrontType[95];
	if(!m_Front)
	{
		return false;
	}

	// Read in the Front size and spacing between chars.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Front[i].left;
		fin >> m_Front[i].right;
		fin >> m_Front[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}


void Front::ReleaseFrontData()
{
	// Release the Front data array.
	if(m_Front)
	{
		delete [] m_Front;
		m_Front = 0;
	}

	return;
}


bool Front::LoadTexture(ID3D11Device* device, const char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new Texture;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Front::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* Front::GetTexture()
{
	return m_Texture->GetTexture();
}


void Front::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Front[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Front[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Front[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Front[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Front[letter].size + 1.0f;
		}
	}

	return;
}