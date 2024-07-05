#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

namespace Core {
	struct Compnent {
		Compnent();
		~Compnent();
		Compnent(const Compnent&);
		Compnent(Compnent&&) noexcept ;
		Compnent& operator=(const Compnent&);
		Compnent& operator=(Compnent&&) noexcept ;

		

	};
	

}
#endif // !COMPONENTS_HPP
