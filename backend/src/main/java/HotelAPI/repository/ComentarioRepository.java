package HotelAPI.repository;

import HotelAPI.model.Comentario;
import org.springframework.data.jpa.repository.JpaRepository;
import java.util.List;

public interface ComentarioRepository extends JpaRepository<Comentario, Integer> {  // Mude de Long para Integer
    List<Comentario> findByPostagem_IdPostagem(Integer idPostagem);  // Mude de int para Integer
}