package HotelAPI.controller;

import HotelAPI.model.Stories;
import HotelAPI.repository.StoriesRepository;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/Stories")
@CrossOrigin("*")
public class StoriesController {

    private final StoriesRepository storiesRepository;

    public StoriesController(StoriesRepository storiesRepository) {
        this.storiesRepository = storiesRepository;
    }

    //Buscar Todos Storiees
    @GetMapping("/AllStories")
    public List<Stories> getAllStories() {
        return storiesRepository.findAll();
    }

    //Buscar Por ID
    @GetMapping("/{id}")
    public Stories FindByIdStories(@PathVariable int id) {
        return storiesRepository.findById(id).orElse(null);
    }

    //Criar Stories
    @PostMapping("/post")
    public Stories createStories(@RequestBody Stories stories) {
        return storiesRepository.save(stories);
    }

    //Deletar Por Id
    @DeleteMapping("/DeletarStories{id}")
    public void deleteStories(@PathVariable int id) {
        storiesRepository.deleteById(id);
    }

    //Put por id
    @PutMapping("/alter{id}")
    public Stories updateStories(@RequestBody Stories stories, @PathVariable int id) {
        return storiesRepository.save(stories);
    }


}
